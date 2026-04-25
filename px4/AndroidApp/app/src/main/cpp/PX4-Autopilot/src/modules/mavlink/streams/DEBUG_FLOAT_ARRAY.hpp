/****************************************************************************
 *
 *   Copyright (c) 2020 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef DEBUG_FLOAT_ARRAY_HPP
#define DEBUG_FLOAT_ARRAY_HPP

#include <uORB/topics/debug_array.h>
#include <uORB/topics/rocket_gnc_status.h>

class MavlinkStreamDebugFloatArray : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamDebugFloatArray(mavlink); }

	static constexpr const char *get_name_static() { return "DEBUG_FLOAT_ARRAY"; }
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_DEBUG_FLOAT_ARRAY; }

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		unsigned size = 0;

		if (_debug_array_sub.advertised()) {
			size += MAVLINK_MSG_ID_DEBUG_FLOAT_ARRAY_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
		}

		if (_rocket_gnc_status_sub.advertised()) {
			size += MAVLINK_MSG_ID_DEBUG_FLOAT_ARRAY_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
		}

		return size;
	}

private:
	explicit MavlinkStreamDebugFloatArray(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _debug_array_sub{ORB_ID(debug_array)};
	uORB::Subscription _rocket_gnc_status_sub{ORB_ID(rocket_gnc_status)};

	bool send() override
	{
		bool sent = false;

		// Forward debug_array (servo feedback id=1)
		debug_array_s debug;

		if (_debug_array_sub.update(&debug)) {
			mavlink_debug_float_array_t msg{};

			msg.time_usec = debug.timestamp;
			msg.array_id = debug.id;
			memcpy(msg.name, debug.name, sizeof(msg.name));
			msg.name[sizeof(msg.name) - 1] = '\0'; // enforce null termination

			for (size_t i = 0; i < debug_array_s::ARRAY_SIZE; i++) {
				msg.data[i] = debug.data[i];
			}

			mavlink_msg_debug_float_array_send_struct(_mavlink->get_channel(), &msg);
			sent = true;
		}

		// Pack rocket_gnc_status into DEBUG_FLOAT_ARRAY id=2
		rocket_gnc_status_s gnc;

		if (_rocket_gnc_status_sub.update(&gnc)) {
			mavlink_debug_float_array_t msg{};

			msg.time_usec = gnc.timestamp;
			msg.array_id = 2;
			strncpy(msg.name, "RktGNC", sizeof(msg.name));
			msg.name[sizeof(msg.name) - 1] = '\0';

			// Pack GNC fields into data array
			msg.data[0]  = (float)gnc.stage;
			msg.data[1]  = gnc.t_flight;
			msg.data[2]  = gnc.q_dyn;
			msg.data[3]  = gnc.q_kgf;
			msg.data[4]  = gnc.pitch_accel_cmd;
			msg.data[5]  = gnc.yaw_accel_cmd;
			msg.data[6]  = gnc.yaw_los_deg;
			msg.data[7]  = gnc.delta_roll;
			msg.data[8]  = gnc.delta_pitch;
			msg.data[9]  = gnc.delta_yaw;
			msg.data[10] = gnc.fin1;
			msg.data[11] = gnc.fin2;
			msg.data[12] = gnc.fin3;
			msg.data[13] = gnc.fin4;
			msg.data[14] = gnc.altitude;
			msg.data[15] = gnc.airspeed;
			msg.data[16] = gnc.rho;
			msg.data[17] = (float)gnc.servo_online_mask;

			// Currently missing from existing topic
			msg.data[18] = gnc.phi;
			msg.data[19] = gnc.wx_filter;
			msg.data[20] = gnc.du_roll;
			msg.data[21] = gnc.out_integ_roll;

			// New fields - attitude angles
			msg.data[22] = gnc.theta;
			msg.data[23] = gnc.psi;
			msg.data[24] = gnc.alpha_est;
			msg.data[25] = gnc.gamma_rad;

			// New fields - guidance frame position
			msg.data[26] = gnc.pos_downrange;
			msg.data[27] = gnc.pos_crossrange;
			msg.data[28] = gnc.vel_downrange;
			msg.data[29] = gnc.vel_down;
			msg.data[30] = gnc.vel_crossrange;

			// New fields - controller state
			msg.data[31] = gnc.wz_filter;
			msg.data[32] = gnc.wy_filter;
			msg.data[33] = gnc.ayb_filter;
			msg.data[34] = gnc.azb_filter;
			msg.data[35] = gnc.out_integ_yaw;
			msg.data[36] = gnc.apc_accum;

			// New fields - PN guidance state
			msg.data[37] = gnc.pn2_b1;
			msg.data[38] = gnc.pn2_b_int;
			msg.data[39] = gnc.bearing_deg;
			msg.data[40] = gnc.target_range_remaining;

			// New fields - event flags
			msg.data[41] = gnc.alpha_protection_active ? 1.0f : 0.0f;
			msg.data[42] = gnc.launched ? 1.0f : 0.0f;

			mavlink_msg_debug_float_array_send_struct(_mavlink->get_channel(), &msg);
			sent = true;
		}

		return sent;
	}
};

#endif // DEBUG_FLOAT_ARRAY_HPP
