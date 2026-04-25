#pragma once

// TCP-to-UDP bridge for MAVLink over USB (ADB forward)
// TCP server on port 5760, forwards to/from MAVLink UDP on localhost:14550

void mavlink_tcp_bridge_start(int tcp_port, int udp_port);
void mavlink_tcp_bridge_stop();
