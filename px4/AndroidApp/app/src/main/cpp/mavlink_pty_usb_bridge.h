/**
 * mavlink_pty_usb_bridge.h
 * PTY master <-> CP210x USB bulk (Silicon Labs) for MAVLink serial (-d) on Android.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool mavlink_pty_usb_bridge_start(int usb_fd, int baud);
void mavlink_pty_usb_bridge_stop();
const char *mavlink_pty_usb_bridge_get_slave_path();

#ifdef __cplusplus
}
#endif
