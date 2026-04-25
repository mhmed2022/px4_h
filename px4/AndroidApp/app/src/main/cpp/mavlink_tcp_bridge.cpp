/**
 * mavlink_tcp_bridge.cpp
 * TCP↔UDP bridge for MAVLink over USB connection.
 * Allows QGroundControl to connect via: adb forward tcp:5760 tcp:5760
 *
 * TCP server (port 5760) ↔ UDP MAVLink (localhost:14550)
 */
#include "mavlink_tcp_bridge.h"

#include <android/log.h>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <string.h>

#define TAG "MavTcpBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static std::atomic<bool> s_bridge_running{false};
static std::thread s_bridge_thread;
static int s_tcp_server_fd = -1;

static void bridge_loop(int tcp_port, int udp_port) {
    // 1. Create TCP server socket
    s_tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_tcp_server_fd < 0) {
        LOGE("TCP socket failed: %s", strerror(errno));
        return;
    }

    int opt = 1;
    setsockopt(s_tcp_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in tcp_addr{};
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_addr.sin_port = htons(tcp_port);

    if (bind(s_tcp_server_fd, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) < 0) {
        LOGE("TCP bind port %d failed: %s", tcp_port, strerror(errno));
        close(s_tcp_server_fd);
        s_tcp_server_fd = -1;
        return;
    }

    listen(s_tcp_server_fd, 1);
    LOGI("TCP bridge listening on port %d → UDP %d", tcp_port, udp_port);

    while (s_bridge_running) {
        // Wait for TCP connection with timeout
        struct pollfd pfd{};
        pfd.fd = s_tcp_server_fd;
        pfd.events = POLLIN;
        int ret = poll(&pfd, 1, 1000);
        if (ret <= 0) continue;

        struct sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int tcp_client_fd = accept(s_tcp_server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (tcp_client_fd < 0) continue;

        LOGI("QGC connected via TCP");

        // 2. Create UDP socket to talk to MAVLink
        int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_fd < 0) {
            LOGE("UDP socket failed");
            close(tcp_client_fd);
            continue;
        }

        // Bind UDP to fixed port 14551 — MAVLink sends heartbeats here via -o 14551
        struct sockaddr_in udp_bind_addr{};
        udp_bind_addr.sin_family = AF_INET;
        udp_bind_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        udp_bind_addr.sin_port = htons(udp_port + 1);  // 14551 — MAVLink target port
        int reuse = 1;
        setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        if (bind(udp_fd, (struct sockaddr*)&udp_bind_addr, sizeof(udp_bind_addr)) < 0) {
            LOGE("UDP bind port %d failed: %s", udp_port + 1, strerror(errno));
            close(udp_fd);
            close(tcp_client_fd);
            continue;
        }
        LOGI("Bridge UDP bound to port %d", udp_port + 1);

        // MAVLink target address
        struct sockaddr_in mavlink_addr{};
        mavlink_addr.sin_family = AF_INET;
        mavlink_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        mavlink_addr.sin_port = htons(udp_port);

        // 3. Bridge loop: TCP ↔ UDP
        char buf[2048];
        struct pollfd fds[2];
        fds[0].fd = tcp_client_fd;
        fds[0].events = POLLIN;
        fds[1].fd = udp_fd;
        fds[1].events = POLLIN;

        while (s_bridge_running) {
            int pr = poll(fds, 2, 1000);
            if (pr < 0) break;
            if (pr == 0) continue;

            // TCP → UDP (QGC → MAVLink)
            if (fds[0].revents & POLLIN) {
                ssize_t n = recv(tcp_client_fd, buf, sizeof(buf), 0);
                if (n <= 0) {
                    LOGI("QGC disconnected");
                    break;
                }
                sendto(udp_fd, buf, n, 0, (struct sockaddr*)&mavlink_addr, sizeof(mavlink_addr));
            }
            if (fds[0].revents & (POLLERR | POLLHUP)) break;

            // UDP → TCP (MAVLink → QGC)
            if (fds[1].revents & POLLIN) {
                ssize_t n = recv(udp_fd, buf, sizeof(buf), 0);
                if (n > 0) {
                    send(tcp_client_fd, buf, n, MSG_NOSIGNAL);
                }
            }
        }

        close(udp_fd);
        close(tcp_client_fd);
    }

    close(s_tcp_server_fd);
    s_tcp_server_fd = -1;
    LOGI("TCP bridge stopped");
}

void mavlink_tcp_bridge_start(int tcp_port, int udp_port) {
    if (s_bridge_running) return;
    s_bridge_running = true;
    s_bridge_thread = std::thread(bridge_loop, tcp_port, udp_port);
}

void mavlink_tcp_bridge_stop() {
    s_bridge_running = false;
    if (s_tcp_server_fd >= 0) {
        close(s_tcp_server_fd);
        s_tcp_server_fd = -1;
    }
    if (s_bridge_thread.joinable()) {
        s_bridge_thread.join();
    }
}
