/**
 * arduino_pty_usb_bridge.h
 * PTY master <-> CH340 USB bulk (Arduino) for actuator output on Android.
 * 
 * Creates a PTY pair. The PX4 driver opens the slave side as a serial port.
 * This bridge reads from PTY master and sends to Arduino via USB (CH340).
 */
#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Start the PTY-to-USB bridge.
 * @param usb_fd Linux USB file descriptor (from UsbDeviceConnection.getFileDescriptor())
 * @param baud Baud rate (typically 115200)
 * @return true on success
 */
bool arduino_pty_usb_bridge_start(int usb_fd, int baud);

/**
 * Stop the bridge and cleanup resources.
 */
void arduino_pty_usb_bridge_stop();

/**
 * Get the PTY slave path (e.g., "/dev/pts/7").
 * The PX4 driver opens this path with open().
 * @return slave path string, or NULL if not running
 */
const char *arduino_pty_usb_bridge_get_slave_path();

#ifdef __cplusplus
}
#endif
