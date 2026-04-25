/**
 * mavlink_pty_usb_bridge.cpp
 * PTY (slave) for PX4 mavlink -d, master bridged to CP2102 USB bulk on Android.
 */
#include "mavlink_pty_usb_bridge.h"

#include <android/log.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/usbdevice_fs.h>
#include <poll.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

#define TAG_PTY "MavPtyUsb"
#define LOGI_PTY(...) __android_log_print(ANDROID_LOG_INFO, TAG_PTY, __VA_ARGS__)
#define LOGE_PTY(...) __android_log_print(ANDROID_LOG_ERROR, TAG_PTY, __VA_ARGS__)

static constexpr uint8_t CP210X_IFC_ENABLE = 0x00;
static constexpr uint8_t CP210X_SET_LINE_CTL = 0x03;
static constexpr uint8_t CP210X_SET_BAUDRATE = 0x1E;
static constexpr uint16_t CP210X_LINE_8N1 = 0x0800;
static constexpr unsigned int CP210X_USB_INTERFACE = 0;
static constexpr uint8_t CP210X_EP_OUT = 0x01;
static constexpr uint8_t CP210X_EP_IN = 0x81;

std::mutex g_bridge_mutex;
std::atomic<bool> g_running{false};
std::thread g_th_pty_to_usb;
std::thread g_th_usb_to_pty;
int g_usb_fd{-1};
int g_pty_master{-1};
std::string g_slave_path;
std::atomic<bool> g_threads_should_run{false};

int usb_control(int fd, uint8_t reqtype, uint8_t request, uint16_t value, uint16_t index,
		void *data, uint16_t len)
{
	struct usbdevfs_ctrltransfer ct {};
	ct.bRequestType = reqtype;
	ct.bRequest = request;
	ct.wValue = value;
	ct.wIndex = index;
	ct.wLength = len;
	ct.timeout = 1000;
	ct.data = data;
	return ioctl(fd, USBDEVFS_CONTROL, &ct);
}

bool cp210x_configure(int fd, unsigned baud)
{
	int r = usb_control(fd, 0x41, CP210X_IFC_ENABLE, 1, CP210X_USB_INTERFACE, nullptr, 0);
	if (r < 0) {
		LOGE_PTY("CP210X_IFC_ENABLE failed: r=%d errno=%d (%s)", r, errno, strerror(errno));
		return false;
	}

	uint8_t baud_le[4];
	baud_le[0] = (uint8_t)(baud & 0xff);
	baud_le[1] = (uint8_t)((baud >> 8) & 0xff);
	baud_le[2] = (uint8_t)((baud >> 16) & 0xff);
	baud_le[3] = (uint8_t)((baud >> 24) & 0xff);
	r = usb_control(fd, 0x41, CP210X_SET_BAUDRATE, 0, CP210X_USB_INTERFACE, baud_le, 4);
	if (r < 0) {
		LOGE_PTY("CP210X_SET_BAUDRATE failed: r=%d errno=%d (%s)", r, errno, strerror(errno));
		return false;
	}

	r = usb_control(fd, 0x41, CP210X_SET_LINE_CTL, CP210X_LINE_8N1, CP210X_USB_INTERFACE, nullptr, 0);
	if (r < 0) {
		LOGE_PTY("CP210X_SET_LINE_CTL failed: r=%d errno=%d (%s)", r, errno, strerror(errno));
		return false;
	}

	LOGI_PTY("CP210x configured: baud=%u", baud);
	return true;
}

bool usb_claim_and_prep(int fd, unsigned baud)
{
	unsigned int iface = CP210X_USB_INTERFACE;
	int r = ioctl(fd, USBDEVFS_CLAIMINTERFACE, &iface);
	if (r < 0) {
		if (errno == EBUSY) {
			LOGI_PTY("USBDEVFS_CLAIMINTERFACE EBUSY — continuing (Java may own interface)");
		} else {
			LOGE_PTY("USBDEVFS_CLAIMINTERFACE failed: errno=%d (%s)", errno, strerror(errno));
			return false;
		}
	}
	if (!cp210x_configure(fd, baud)) {
		if (r == 0) ioctl(fd, USBDEVFS_RELEASEINTERFACE, &iface);
		return false;
	}
	return true;
}

void usb_release_if_needed(int fd)
{
	if (fd < 0) return;
	unsigned int iface = CP210X_USB_INTERFACE;
	ioctl(fd, USBDEVFS_RELEASEINTERFACE, &iface);
}

int usb_bulk_write(int fd, const void *buf, int len)
{
	struct usbdevfs_bulktransfer bulk {};
	bulk.ep = CP210X_EP_OUT;
	bulk.len = len;
	bulk.timeout = 200;
	bulk.data = const_cast<void *>(buf);
	return ioctl(fd, USBDEVFS_BULK, &bulk);
}

int usb_bulk_read(int fd, void *buf, int maxlen)
{
	struct usbdevfs_bulktransfer bulk {};
	bulk.ep = CP210X_EP_IN;
	bulk.len = maxlen;
	bulk.timeout = 100;
	bulk.data = buf;
	return ioctl(fd, USBDEVFS_BULK, &bulk);
}

void thread_pty_to_usb_fn()
{
	char buf[4096];
	while (g_threads_should_run.load()) {
		if (g_pty_master < 0 || g_usb_fd < 0) break;
		struct pollfd pfd {};
		pfd.fd = g_pty_master;
		pfd.events = POLLIN;
		int pr = poll(&pfd, 1, 100);
		if (pr < 0) { if (errno == EINTR) continue; break; }
		if (pr == 0) continue;
		ssize_t n = read(g_pty_master, buf, sizeof(buf));
		if (n <= 0) {
			if (n < 0 && (errno == EAGAIN || errno == EINTR)) continue;
			LOGI_PTY("pty->usb read ended n=%zd errno=%d", n, errno);
			break;
		}
		int off = 0;
		int timeout_retries = 0;
		while (off < (int)n && g_threads_should_run.load()) {
			int w = usb_bulk_write(g_usb_fd, buf + off, (int)n - off);
			if (w < 0) {
				if (errno == ETIMEDOUT) {
					// Retry: breaking mid-packet truncates the MAVLink frame and
					// corrupts the stream. Retry a bounded number of times; if the
					// endpoint is truly stuck, mark the bridge dead.
					if (++timeout_retries < 10) continue;
					LOGE_PTY("usb bulk write stuck after %d ETIMEDOUT retries — marking fatal", timeout_retries);
					g_threads_should_run.store(false); break;
				}
				LOGE_PTY("usb bulk write fatal errno=%d (%s)", errno, strerror(errno));
				g_threads_should_run.store(false); break;
			}
			timeout_retries = 0;
			off += w;
		}
	}
}

void thread_usb_to_pty_fn()
{
	char buf[4096];
	while (g_threads_should_run.load()) {
		if (g_pty_master < 0 || g_usb_fd < 0) break;
		int n = usb_bulk_read(g_usb_fd, buf, sizeof(buf));
		if (n < 0) { if (errno == ETIMEDOUT || errno == EAGAIN) continue; LOGE_PTY("usb bulk read errno=%d (%s)", errno, strerror(errno)); break; }
		if (n == 0) continue;
		int off = 0;
		while (off < n && g_threads_should_run.load()) {
			ssize_t w = write(g_pty_master, buf + off, n - off);
			if (w < 0) { if (errno == EAGAIN || errno == EINTR) continue; LOGI_PTY("usb->pty write errno=%d", errno); g_threads_should_run.store(false); break; }
			off += (int)w;
		}
	}
}

bool create_pty()
{
	g_pty_master = posix_openpt(O_RDWR | O_NOCTTY | O_CLOEXEC);
	if (g_pty_master < 0) { LOGE_PTY("posix_openpt failed: errno=%d (%s)", errno, strerror(errno)); return false; }
	if (grantpt(g_pty_master) < 0) { LOGE_PTY("grantpt failed"); close(g_pty_master); g_pty_master = -1; return false; }
	if (unlockpt(g_pty_master) < 0) { LOGE_PTY("unlockpt failed"); close(g_pty_master); g_pty_master = -1; return false; }
	char pts_path[256];
	if (ptsname_r(g_pty_master, pts_path, sizeof(pts_path)) != 0) { LOGE_PTY("ptsname_r failed"); close(g_pty_master); g_pty_master = -1; return false; }
	g_slave_path = pts_path;
	LOGI_PTY("PTY slave path: %s", g_slave_path.c_str());
	return true;
}

static void bridge_stop_unlocked()
{
	if (!g_running.load()) { g_slave_path.clear(); return; }
	g_threads_should_run.store(false);
	// NOTE: previously called shutdown(g_pty_master, SHUT_RDWR) here, but PTY
	// masters are not sockets — shutdown() returns ENOTSOCK. Both bridge
	// threads already poll g_threads_should_run on a ≤100ms cadence (poll
	// timeout / bulk read timeout), so clearing the flag unblocks them.
	if (g_th_pty_to_usb.joinable()) g_th_pty_to_usb.join();
	if (g_th_usb_to_pty.joinable()) g_th_usb_to_pty.join();
	if (g_pty_master >= 0) { close(g_pty_master); g_pty_master = -1; }
	usb_release_if_needed(g_usb_fd);
	g_usb_fd = -1;
	g_slave_path.clear();
	g_running.store(false);
	LOGI_PTY("bridge stopped");
}

extern "C" bool mavlink_pty_usb_bridge_start(int usb_fd, int baud)
{
	std::lock_guard<std::mutex> lock(g_bridge_mutex);
	if (g_running.load()) bridge_stop_unlocked();
	if (usb_fd < 0) { LOGE_PTY("invalid usb_fd"); return false; }
	if (baud < 9600 || baud > 3000000) { LOGE_PTY("baud out of range: %d", baud); return false; }
	if (!usb_claim_and_prep(usb_fd, (unsigned)baud)) return false;
	if (!create_pty()) { usb_release_if_needed(usb_fd); return false; }
	g_usb_fd = usb_fd;
	g_threads_should_run.store(true);
	g_th_pty_to_usb = std::thread(thread_pty_to_usb_fn);
	g_th_usb_to_pty = std::thread(thread_usb_to_pty_fn);
	g_running.store(true);
	LOGI_PTY("bridge started (usb_fd=%d baud=%d)", usb_fd, baud);
	return true;
}

extern "C" void mavlink_pty_usb_bridge_stop()
{
	std::lock_guard<std::mutex> lock(g_bridge_mutex);
	bridge_stop_unlocked();
}

extern "C" const char *mavlink_pty_usb_bridge_get_slave_path()
{
	if (g_slave_path.empty()) return nullptr;
	return g_slave_path.c_str();
}
