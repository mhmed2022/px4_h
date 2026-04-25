/****************************************************************************
 *
 *   Copyright (c) 2026 PX4 Development Team. All rights reserved.
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

/**
 * @file RocketGNC.cpp
 * Rocket M130 GNC — M130AutopilotSystem + M130GuidanceSystem + XFinMixer
 * Ported from 6DOF_v2_130/dynamics/m130_autopilot.py and m130_guidance.py
 */

#include "RocketGNC.hpp"

#include <px4_platform_common/getopt.h>
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>

// ===================================================================
//  Constructor / Destructor
// ===================================================================

RocketGNC::RocketGNC() :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers)
{
	_vehicle_torque_setpoint_pub.advertise();
	_vehicle_thrust_setpoint_pub.advertise();
	_actuator_outputs_sim_pub.advertise();
	_rocket_gnc_status_pub.advertise();
	parameters_update(true);
}

RocketGNC::~RocketGNC()
{
	ScheduleClear();
	perf_free(_loop_perf);
}

bool RocketGNC::init()
{
	// In HITL mode on real hardware, read ground-truth topics instead of
	// EKF2 estimates.  HIL_STATE_QUATERNION → mavlink receiver publishes
	// to vehicle_attitude_groundtruth / vehicle_local_position_groundtruth.
	// Without this, rocket_gnc uses EKF2 which may not converge in time.
	// HITL: works on all platforms (Android, NuttX, POSIX)
	int32_t hitl_val = 0;
	param_get(param_find("SYS_HITL"), &hitl_val);

	if (hitl_val == 1) {
		_vehicle_attitude_sub       = uORB::Subscription{ORB_ID(vehicle_attitude_groundtruth)};
		_vehicle_local_position_sub = uORB::Subscription{ORB_ID(vehicle_local_position_groundtruth)};
		_hitl_mode = true;
		PX4_INFO("HITL mode: using groundtruth topics + velocity-based launch detection");
	}
	ScheduleOnInterval(10_ms);   // 100 Hz
	return true;
}

void RocketGNC::parameters_update(bool force)
{
	if (_parameter_update_sub.updated() || force) {
		parameter_update_s pupdate;
		_parameter_update_sub.copy(&pupdate);
		updateParams();
	}
}

// ===================================================================
//  Stage determination
// ===================================================================

int RocketGNC::_determineStage(float t, float yaw_los_deg)
{
	if (!_stage1_completed) {
		if (fabsf(yaw_los_deg) > 11.0f) {
			_stage1_completed = true;
			_stage1_end_time  = t;
			PX4_INFO("rocket_gnc: stage 1→2 at t=%.2fs (|yaw_los|=%.1f° > 11°)",
				 (double)t, (double)fabsf(yaw_los_deg));

		} else if (t >= _param_t_stg1.get()) {
			_stage1_completed = true;
			_stage1_end_time  = t;
			PX4_INFO("rocket_gnc: stage 1→2 at t=%.2fs (fallback t_stg1)", (double)t);

		} else {
			return 1;
		}
	}

	return 2;
}

// ===================================================================
//  Gain scheduling  (PilotGain)
// ===================================================================

void RocketGNC::_pilotGain(int stage, float q_kgf,
                            float &K_w_pitch, float &K_w_yaw, float &K_w_roll,
                            float &K_p_integ_acc, float &K_i_integ_acc,
                            float &K_p_integ_roll, float &K_i_integ_roll,
                            float &out_integ_limit, float &apc_limit,
                            float &Acc_sat)
{
	const float q = fmaxf(q_kgf, 0.05f);  // guard against division by zero at low speed

	out_integ_limit = 0.115f / q;
	apc_limit       = 2.5f  * q;
	Acc_sat         = q / 2.0f;

	const float K_acc_2_0 =  0.0115f;
	const float K_acc_2_1 = -0.0656f;

	if (stage == 1) {
		K_p_integ_acc = 0.0f;

		if (q < 1.7f) {
			K_i_integ_acc = 0.7f * 1.5f * 0.5f * (-0.082f);   // ≈ -0.04305

		} else {
			K_i_integ_acc = (0.5f * (-0.082f)) / (2.0f * q);   // = -0.0205/q
		}

	} else {   // stage 2
		if (q < 2.0f) {
			K_p_integ_acc = 1.0f   * K_acc_2_0;    // = 0.0115
			K_i_integ_acc = 1.31f  * K_acc_2_1;    // ≈ -0.085936

		} else {
			K_p_integ_acc = -0.035f * K_acc_2_0;   // ≈ -0.0004025
			K_i_integ_acc =  1.721f * K_acc_2_1;   // ≈ -0.112898
		}

	}

	K_w_pitch      = 0.0086f * q;
	K_w_yaw        = 0.1f * 0.0582f * 1.0f;   // = 0.00582 (constant)
	K_w_roll       = 0.012f;                   // rate damping  (was 0.015 — reduced to not oppose correction)
	K_p_integ_roll = 0.05f;                    // proportional (was 0.02  — ×2.5, stronger instant response)
	K_i_integ_roll = 0.01f;                    // integral     (was 0.003 — ×3, faster steady-state trim)
}

// ===================================================================
//  Speed scaler
// ===================================================================

float RocketGNC::_calcSpeedScaler(float vm)
{
	const float scaling_speed = 170.0f;
	const float airspeed_max  = 278.0f;
	const float airspeed_min  = 30.0f;

	const float scale_min = fminf(0.5f, scaling_speed / (2.0f * airspeed_max));
	const float scale_max = fmaxf(2.0f, scaling_speed / (0.7f * airspeed_min));

	float speed_scaler = (vm > 0.0001f) ? (scaling_speed / vm) : scale_max;
	return math::constrain(speed_scaler, scale_min, scale_max);
}

// ===================================================================
//  Altitude hold autopilot  (stage 1)
// ===================================================================

float RocketGNC::_altitudeAutopilot(float sp, float h_fb, float q_kgf,
                                     float ss, float dt, float t)
{
	const float K_h_1 =  q_kgf * 0.15075f;
	const float K_h_2 =  0.5f * ss * 4.0f * 0.017f;
	const float K_h_3 =  0.25f;

	const float h_error = (sp - h_fb) * 1.8f;

	_alt_int_accum += K_h_2 * h_error * dt;
	_alt_int_accum  = math::constrain(_alt_int_accum, -1.5f, 1.5f);

	float ep = K_h_1 * h_error;
	float ei = 3.0f * K_h_2 * _alt_int_accum;
	float ed = (K_h_3 * 1.7f * (h_error - _alt_prev_herror)) / ((dt > 1e-9f) ? dt : 1e-9f);

	const float q_filt = fmaxf(q_kgf, 1.0f);  // minimum filter weight to prevent no-filtering at low speed
	_alt_ed_filt = (ed + q_filt * _alt_ed_filt) / (q_filt + 1.0f);

	const float lim_p_d = 0.3f;
	_alt_ed_filt = math::constrain(_alt_ed_filt, -lim_p_d, lim_p_d);
	ep           = math::constrain(ep, -0.5f * lim_p_d, 0.5f * lim_p_d);

	float u_ae = ep + ei + _alt_ed_filt;

	_alt_prev_herror = h_error;

	const float alti_lim = math::radians(10.0f);
	float u_cmd = math::constrain(u_ae, -alti_lim, alti_lim) * -1.0f;

	if (t < 0.05f) { u_cmd = 0.0f; }

	return u_cmd;
}

// ===================================================================
//  Pitch acceleration autopilot  (stage 2+)
// ===================================================================

float RocketGNC::_apcAutopilot(float fz_fb, float q_fb, float ac_pitch,
                                float q_kgf, float dt)
{
	const float K_acc_0 =  1.0f * 1.0f  *  0.0010981081f;
	const float K_acc_1 = -1.0f * 1.5f  *  0.09587f;
	const float K_acc_2 =  1.0f * 0.4581f;

	const float apc_error = fz_fb - ac_pitch;

	const float q = q_kgf;
	_apc_accum += apc_error * dt * 1.1f;
	_apc_accum  = math::constrain(_apc_accum, -5.5f * q, 5.5f * q);

	float u_ae = K_acc_0 * apc_error
	             + K_acc_1 * _apc_accum
	             + K_acc_2 * q_fb;

	return math::constrain(u_ae, -math::radians(13.0f), math::radians(13.0f));
}

// ===================================================================
//  Guidance — yaw command  (t < 5 s)
// ===================================================================

float RocketGNC::_yawComm(float Vzm, float Zm)
{
	const float k_guidance_yaw = _param_k_yaw.get();
	const float k_vz           = _param_k_vz.get();

	const float vz_err = -k_vz * Vzm;
	const float z_err  = -Zm;   // crossrange target is 0 in rotated frame

	float ayc = 0.8f * k_guidance_yaw * (z_err + vz_err);

	const float ayc_limit = _param_ayc_lim.get();
	if (fabsf(ayc) > ayc_limit) {
		ayc = ayc_limit * (ayc / fabsf(ayc));
	}

	return ayc;
}

// ===================================================================
//  Guidance — yaw PN  (t >= 5 s)
// ===================================================================

float RocketGNC::_yawComdd2(float Xm, float Zm, float Vxm, float Vzm, float t)
{
	float dx_tr = _target_downrange - Xm;
	float dz_tr = -Zm;   // crossrange target is 0 in rotated frame

	if (dx_tr < 300.0f) { dx_tr = 300.0f; }
	if (fabsf(dx_tr) <= 1.0f) { dx_tr = 1.0f; }

	float rxz2 = dx_tr * dx_tr + dz_tr * dz_tr;
	if (rxz2 < 10000.0f) { rxz2 = 10000.0f; }

	if (Vxm < 0.0f) { return 0.0f; }

	const float Vne     = sqrtf(Vxm * Vxm + Vzm * Vzm);
	const float qe_dot2 = (dz_tr * Vxm - dx_tr * Vzm) / rxz2;

	float ayc = 0.25f * qe_dot2 * Vne;

	const float ayc_limit = _param_ayc_lim.get();
	if (fabsf(ayc) > ayc_limit) {
		ayc = ayc_limit * (ayc / fabsf(ayc));
	}

	ayc = math::constrain(ayc, -2.0f, 2.0f);
	return ayc;
}

// ===================================================================
//  Guidance — biased proportional navigation pitch  (pn2)
// ===================================================================

float RocketGNC::_pn2(float Xm, float Ym, float Vxm, float Vym, float Vzm,
                       float phi, const Quatf &quat, float t, float apc_limit)
{
	float dxi = _target_downrange - Xm;
	float dyi = -(_param_ytrgt.get() - Ym);   // altitude target — independent of bearing

	const float impact_angle1 = 0.5f * _param_impact_ang.get();   // 0.5 * -30 = -15 deg

	if (dxi < 0.0f) { return 0.0f; }

	float rxy = sqrtf(dxi * dxi + dyi * dyi);
	if (rxy < 10.0f) { rxy = 10.0f; }
	if (fabsf(dxi) < 1.0f) { dxi = 1.0f; }

	const float lambda_angle = atan2f(dyi, dxi);
	const float gama         = atan2f(Vym, Vxm);  // flight path angle (NED convention)

	float Vm_t = sqrtf(Vxm * Vxm + Vym * Vym + Vzm * Vzm);
	if (Vm_t < 1.0f) { Vm_t = 1.0f; }

	const float Npn     = _param_npn.get();
	const float tau_pn1 = _param_tau_pn1.get();
	const float mag_g   = 9.80665f;
	const float d2r     = M_PI_F / 180.0f;

	// Bias initialisation (first call only)
	if (!_pn2_initialized) {
		_pn2_initialized = true;
		_pn2_t_old       = t;
		_pn2_Bref        = (1.0f - Npn) * impact_angle1 * d2r
		                   - gama + Npn * lambda_angle;

	} else {
		const float E_bias = _pn2_Bref - _pn2_b_int;
		float b1 = E_bias / tau_pn1;

		const float b1_max = apc_limit * mag_g / Vm_t;
		if (fabsf(b1) > b1_max) {
			b1 = (b1 > 0.0f) ? b1_max : -b1_max;
		}

		const float dt_pn = t - _pn2_t_old;
		_pn2_b_int += b1 * dt_pn;
		_pn2_t_old  = t;
		_pn2_b1     = b1;
	}

	// PN guidance command
	const float sin_diff  = sinf(gama - lambda_angle);
	const float lambda_dot = (-Vm_t * sin_diff) / rxy;
	const float gama_dot  = Npn * lambda_dot;
	const float ay_com    = gama_dot * Vm_t + _pn2_b1 * Vm_t;

	// Gravity compensation in body frame via quaternion DCM
	// C_bn (body → NED) = Quatf rotation matrix
	const matrix::Dcmf C_bn(quat);
	const Vector3f g_ned(0.0f, 0.0f, mag_g);    // NED: down is +z
	const Vector3f g_body = C_bn.transpose() * g_ned;

	const float cos_phi      = cosf(phi);
	const float sin_phi      = sinf(phi);
	const float gravity_comp = cos_phi * g_body(2) - sin_phi * g_body(1);

	float ac_pitch = (ay_com - gravity_comp) / mag_g;

	const float pitch_limit = _param_apc_lim.get();
	ac_pitch = math::constrain(ac_pitch, -pitch_limit, pitch_limit);

	return ac_pitch;
}

// ===================================================================
//  Main Run loop
// ===================================================================

void RocketGNC::Run()
{
	if (should_exit()) {
		ScheduleClear();
		exit_and_cleanup();
		return;
	}

	perf_begin(_loop_perf);

	parameters_update();

	// ---------------------------------------------------------------
	// Read sensor data
	// ---------------------------------------------------------------
	sensor_combined_s sc{};
	_sensor_combined_sub.copy(&sc);

	vehicle_attitude_s att{};
	_vehicle_attitude_sub.copy(&att);

	vehicle_local_position_s lpos{};
	_vehicle_local_position_sub.copy(&lpos);

	vehicle_air_data_s air{};
	_vehicle_air_data_sub.copy(&air);

	// ---------------------------------------------------------------
	// Arming detection — capture position origin on arming transition
	// ---------------------------------------------------------------
	vehicle_status_s vstatus{};
	_vehicle_status_sub.copy(&vstatus);
	const bool is_armed = (vstatus.arming_state == vehicle_status_s::ARMING_STATE_ARMED);

	if (is_armed && !_armed) {
		_arm_origin_x = lpos.x;
		_arm_origin_y = lpos.y;
		_arm_origin_z = lpos.z;
		PX4_INFO("rocket_gnc: armed — origin set to (%.2f, %.2f, %.2f)",
			 (double)_arm_origin_x, (double)_arm_origin_y, (double)_arm_origin_z);

		// Reset all flight state so re-arm starts clean
		_launched          = false;
		_launch_dv         = 0.0f;
		_time_launch       = 0;
		_stage1_completed  = false;
		_stage1_end_time   = 0.0f;
		_pn2_initialized   = false;
		_pn2_Bref = _pn2_b_int = _pn2_b1 = _pn2_t_old = 0.0f;
		_apc_accum         = 0.0f;
		_alt_int_accum     = 0.0f;
		_alt_prev_herror   = 0.0f;
		_alt_ed_filt       = 0.0f;
		_Wx_filter = _Wy_filter = _Wz_filter = 0.0f;
		_Ayb_filter = _Azb_filter = 0.0f;
		_out_integ_roll = _out_integ_yaw = 0.0f;
		_prev_yaw_los_deg  = 0.0f;
		// Compute bearing to target from rocket heading at arming.
		// XTRGT = range to target (m), the rocket nose direction = bearing.
		// This way the operator just points the rocket at the target and
		// sets the distance — no need to know North/East coordinates.
		const Quatf q_arm(att.q[0], att.q[1], att.q[2], att.q[3]);
		const Eulerf euler_arm(q_arm);
		const float bearing = euler_arm.psi();     // yaw = heading at arming
		_target_downrange   = _param_xtrgt.get();  // XTRGT = range (m)
		_cos_bearing = cosf(bearing);
		_sin_bearing = sinf(bearing);
		PX4_INFO("rocket_gnc: heading=%.1f deg  target range=%.0f m",
			 (double)(bearing * 180.0f / M_PI_F), (double)_target_downrange);

		if (_param_gnd_test.get() > 0) {
			mavlink_log_critical(&_mavlink_log_pub, "WARNING: GROUND TEST MODE ACTIVE");
		}
	}

	if (!is_armed && _armed) {
		_arm_origin_x = 0.0f;
		_arm_origin_y = 0.0f;
		_arm_origin_z = 0.0f;
	}

	_armed = is_armed;

	const hrt_abstime now = hrt_absolute_time();

	// ---------------------------------------------------------------
	// Pre-launch refinement — continuously improve origin, bearing,
	// and filters while the rocket sits on the rail (armed, not launched).
	// EKF2 converges over time, so the last value before ignition is best.
	// ---------------------------------------------------------------
	if (_armed && !_launched && sc.timestamp > 0) {
		// Update origin whenever EKF2 has a valid position fix
		if (lpos.xy_valid && lpos.z_valid) {
			_arm_origin_x = lpos.x;
			_arm_origin_y = lpos.y;
			_arm_origin_z = lpos.z;

			// Update bearing from latest heading estimate
			const Quatf q_pre(att.q[0], att.q[1], att.q[2], att.q[3]);
			const Eulerf euler_pre(q_pre);
			const float bearing = euler_pre.psi();
			_cos_bearing = cosf(bearing);
			_sin_bearing = sinf(bearing);
		}

		// Pre-load IIR filters with live sensor values so they are
		// primed the instant launch is detected (no cold-start transient)
		const Quatf q_filt(att.q[0], att.q[1], att.q[2], att.q[3]);
		const float phi_pre = Eulerf(q_filt).phi();
		const float cos_phi_pre = cosf(phi_pre);
		const float sin_phi_pre = sinf(phi_pre);
		_Wx_filter  = sc.gyro_rad[0];
		_Wy_filter  = -sc.gyro_rad[2];
		_Wz_filter  = sc.gyro_rad[1];
		_Ayb_filter = cos_phi_pre * sc.accelerometer_m_s2[2] / 9.8f
			      - sin_phi_pre * sc.accelerometer_m_s2[1] / 9.8f;
		_Azb_filter = sin_phi_pre * sc.accelerometer_m_s2[2] / 9.8f
			      + cos_phi_pre * sc.accelerometer_m_s2[1] / 9.8f;
	}

	// ---------------------------------------------------------------
	// Launch detection
	// ---------------------------------------------------------------
	if (!_launched && _armed && sc.timestamp > 0) {
		const float ax = sc.accelerometer_m_s2[0];   // longitudinal (body-X) specific force

		if (ax > 1.0f * 9.80665f) {
			_launch_dv += ax * 0.01f;       // accumulate delta-V (100 Hz step)
		} else {
			_launch_dv = 0.0f;              // reset on drop — spike rejected
		}

		// HITL: also detect launch from groundtruth velocity (sensor_combined
		// may contain phone's real sensor data instead of HIL_SENSOR data)
		if (_hitl_mode && lpos.v_xy_valid) {
			float speed = sqrtf(lpos.vx * lpos.vx + lpos.vy * lpos.vy + lpos.vz * lpos.vz);
			if (speed > 5.0f) {
				_launch_dv = 3.0f;  // force launch detection threshold
			}
		}

		if (_launch_dv > 2.0f) {                // sustained thrust confirmed
			_launched    = true;
			_time_launch = now;
			PX4_WARN(">>> LAUNCH DETECTED <<<  ax=%.1f m/s2", (double)ax);
			mavlink_log_critical(&_mavlink_log_pub, "LAUNCH DETECTED ax=%.1f m/s2", (double)ax);
			PX4_INFO("  SET_ALT=%.1f T_STG1=%.1f T_CTRL=%.2f",
				 (double)_param_set_alt.get(), (double)_param_t_stg1.get(),
				 (double)_param_t_ctrl.get());
			PX4_INFO("  NPN=%.1f IMP_ANG=%.1f TAU_PN1=%.1f",
				 (double)_param_npn.get(), (double)_param_impact_ang.get(),
				 (double)_param_tau_pn1.get());
			PX4_INFO("  XTRGT=%.0f YTRGT=%.0f ZTRGT=%.0f MAX_DEFL=%.3f",
				 (double)_param_xtrgt.get(), (double)_param_ytrgt.get(),
				 (double)_param_ztrgt.get(), (double)_param_max_defl.get());
			PX4_INFO("  bearing=%.1f deg  range=%.0f m",
				 (double)(atan2f(_sin_bearing, _cos_bearing) * 180.0f / M_PI_F),
				 (double)_target_downrange);
			PX4_INFO("  rho=%.4f alt=%.1f",
				 (double)air.rho, (double)(-lpos.z));
		}
	}

	// Flight time (s)
	const float t  = _launched ? (float)(now - _time_launch) * 1e-6f : 0.0f;
	const float dt = 0.01f;   // 100 Hz fixed step

	// Dynamic pressure
	const float v2     = lpos.vx * lpos.vx + lpos.vy * lpos.vy + lpos.vz * lpos.vz;
	const float vm     = sqrtf(v2);
	const float q_dyn  = (air.rho > 0.0f) ? (0.5f * air.rho * v2) : 0.0f;
	float q_kgf  = q_dyn / 10000.0f;   // internal scaling (≈9.8× physical kgf/cm²; all gains tuned to this scale)

	// Ground test mode: override q_kgf so autopilot gains are meaningful on the ground
	if (_param_gnd_test.get() > 0 && _launched) {
		q_kgf = fmaxf(q_kgf, 1.0f);   // simulate ~160 m/s airflow
	}

	// Quaternion from attitude
	const Quatf quat(att.q[0], att.q[1], att.q[2], att.q[3]);
	const Eulerf euler(quat);
	const float phi = euler.phi();   // roll angle

	// Position relative to arm origin in NED
	const float pos_north = lpos.x - _arm_origin_x;
	const float pos_east  = lpos.y - _arm_origin_y;

	// Rotate NED → downrange/crossrange using bearing to target
	const float Xm  =  _cos_bearing * pos_north + _sin_bearing * pos_east;   // downrange
	const float Ym  = -(lpos.z - _arm_origin_z);                              // up = -down
	const float Zm  = -_sin_bearing * pos_north + _cos_bearing * pos_east;    // crossrange

	// Velocity in rotated frame
	const float Vxm =  _cos_bearing * lpos.vx + _sin_bearing * lpos.vy;   // downrange
	const float Vym =  lpos.vz;                                            // down (NED)
	const float Vzm = -_sin_bearing * lpos.vx + _cos_bearing * lpos.vy;   // crossrange

	// ---------------------------------------------------------------
	// Yaw LOS angle (azimuth to target from current position)
	// ---------------------------------------------------------------
	float yaw_los_deg = _prev_yaw_los_deg;

	if (_launched) {
		const float dx_los = _target_downrange - Xm;
		const float dz_los = -Zm;   // crossrange target is 0 in rotated frame
		yaw_los_deg = atan2f(dz_los, (fabsf(dx_los) > 1.0f) ? dx_los : 1.0f)
		              * (180.0f / M_PI_F);
		_prev_yaw_los_deg = yaw_los_deg;
	}

	// ---------------------------------------------------------------
	// Stage determination (uses previous-step yaw_los_deg to avoid
	// circular dependency with guidance)
	// ---------------------------------------------------------------
	const int stage = _launched ? _determineStage(t, yaw_los_deg) : 1;

	// ---------------------------------------------------------------
	// Guidance — compute pitch and yaw acceleration commands
	// ---------------------------------------------------------------
	float ac_pitch = 0.0f;
	float ayc      = 0.0f;

	if (_launched && t > _param_t_ctrl.get()) {
		// apc_limit from _pilot_gain (2.5 * q_kgf)
		const float apc_limit_g = 2.5f * q_kgf;

		ac_pitch = _pn2(Xm, Ym, Vxm, Vym, Vzm, phi, quat, t, apc_limit_g);

		ayc = (t < 5.0f)
		      ? _yawComm(Vzm, Zm)
		      : _yawComdd2(Xm, Zm, Vxm, Vzm, t);
	}

	// ---------------------------------------------------------------
	// Autopilot — compute fin deflection commands
	// ---------------------------------------------------------------
	float delta_roll  = 0.0f;
	float delta_pitch = 0.0f;
	float delta_yaw   = 0.0f;

	if (_launched) {
		// Rail time: filters are already primed by pre-launch refinement,
		// just keep integrators at zero until control activates at T_CTRL
		if (t <= _param_t_ctrl.get()) {
			_out_integ_roll = _out_integ_yaw = 0.0f;

		} else {
			// Gain scheduling
			float K_w_pitch, K_w_yaw, K_w_roll;
			float K_p_integ_acc, K_i_integ_acc;
			float K_p_integ_roll, K_i_integ_roll;
			float out_integ_limit, apc_limit, Acc_sat;

			_pilotGain(stage, q_kgf,
			           K_w_pitch, K_w_yaw, K_w_roll,
			           K_p_integ_acc, K_i_integ_acc,
			           K_p_integ_roll, K_i_integ_roll,
			           out_integ_limit, apc_limit, Acc_sat);

			// Roll saturation
			float Roll_sat;
			const float q = q_kgf;

			if (q < 15.0f) {
				Roll_sat = 3.0f * (0.1396f + 0.03499f * q);
			} else {
				Roll_sat = 3.0f * 0.6632f;
			}

			Roll_sat *= 7.0f;   // match Python reference — allow controller to see full error

			// Speed scaler
			const float ss = _calcSpeedScaler(vm);

			// Angular rates from sensor_combined (rad/s)
			// Mapping: Wx=roll(p), Wy=body-z yaw, Wz=body-y pitch
			const float Wx =  sc.gyro_rad[0];    // roll rate
			const float Wy = -sc.gyro_rad[2];    // body-z → yaw sign flip
			const float Wz =  sc.gyro_rad[1];    // body-y pitch rate

			// IIR filter: (new + 3*old) / 4
			_Wx_filter = (Wx + 3.0f * _Wx_filter) / 4.0f;
			_Wy_filter = (Wy + 3.0f * _Wy_filter) / 4.0f;
			_Wz_filter = (Wz + 3.0f * _Wz_filter) / 4.0f;

			// Body acceleration filter  (m/s² → g, with roll compensation)
			const float cos_phi = cosf(phi);
			const float sin_phi = sinf(phi);

			// From m130_autopilot.py:
			//   Ay_body = accelerometer[2]  → sc.accelerometer_m_s2[2]
			//   Az_body = accelerometer[1]  → sc.accelerometer_m_s2[1]
			const float Ay_body = sc.accelerometer_m_s2[2];
			const float Az_body = sc.accelerometer_m_s2[1];

			const float az_b = sin_phi * Ay_body / 9.8f + cos_phi * Az_body / 9.8f;
			_Azb_filter = (az_b + 3.0f * _Azb_filter) / 4.0f;

			const float ay_b = cos_phi * Ay_body / 9.8f - sin_phi * Az_body / 9.8f;
			_Ayb_filter = (ay_b + 3.0f * _Ayb_filter) / 4.0f;

			// Roll-compensated yaw rate (execute frame)
			const float wyr = -sin_phi * _Wz_filter + cos_phi * _Wy_filter;

			const float wy_compensate = K_w_yaw * wyr;
			const float wx_compensate = K_w_roll * _Wx_filter;

			// ---- Roll channel ----
			const float phic    = 0.0f;
			float du_roll       = phic - phi;
			du_roll             = math::constrain(du_roll, -Roll_sat, Roll_sat);
			_du_roll            = du_roll;

			_out_integ_roll += K_i_integ_roll * du_roll * dt;
			_out_integ_roll  = math::constrain(_out_integ_roll, -0.2f, 0.2f);

			const float delac = 3.0f * ((_out_integ_roll + K_p_integ_roll * du_roll)
			                            - wx_compensate);

			// ---- Yaw channel ----
			const float du_yaw = math::constrain(ayc, -2.0f * Acc_sat, 2.0f * Acc_sat);

			_out_integ_yaw += K_i_integ_acc * du_yaw * dt;
			_out_integ_yaw  = math::constrain(_out_integ_yaw, -out_integ_limit, out_integ_limit);

			float delrc0 = (_out_integ_yaw + K_p_integ_acc * du_yaw) - wy_compensate;

			// ---- Pitch channel ----
			float delec0 = 0.0f;

			if (stage == 1) {
				const float set_alt = fmaxf(_param_set_alt.get(), 10.0f); // guard div-by-zero
				delec0 = _altitudeAutopilot(1.0f, Ym / set_alt,
				                            q_kgf, ss, dt, t);
			} else {
				delec0 = _apcAutopilot(_Ayb_filter,
				                       0.0086f * q_kgf * _Wz_filter,
				                       ac_pitch, q_kgf, dt);
			}

			// Sign flip (matches Python: delec0 *= -1, delrc0 *= -1)
			delec0 *= -1.0f;
			delrc0 *= -1.0f;

			// Body → execute frame transform (roll-stabilized frame)
			// Pitch/Yaw commands are always applied — roll coupling handled by
			// the cos/sin transform, which is the internationally accepted approach.
			const float delec = delec0 * cos_phi - delrc0 * sin_phi;
			const float delrc = delrc0 * cos_phi + delec0 * sin_phi;

			delta_roll  = -delac;
			delta_pitch = -delec;
			delta_yaw   = -delrc;
		}
	}

	// ---------------------------------------------------------------
	// XFin normalized commands — for logging and HITL/SITL actuator output.
	// Physical fin numbering: CS0=TR(fin1), CS1=BR(fin2), CS2=BL(fin3), CS3=TL(fin4)
	//
	// X-fin mixing equations (physical order: fin1=TR, fin2=BR, fin3=BL, fin4=TL):
	// Positive command = clockwise rotation from rear view:
	//   TR/BR: trailing edge DOWN,  BL/TL: trailing edge UP
	//
	//   fin[0] = CS0=TR: τ_roll - τ_pitch + τ_yaw
	//   fin[1] = CS1=BR: τ_roll - τ_pitch - τ_yaw
	//   fin[2] = CS2=BL: τ_roll + τ_pitch - τ_yaw
	//   fin[3] = CS3=TL: τ_roll + τ_pitch + τ_yaw
	//   where τ_axis = delta_axis / max_defl  ∈ [-1, +1]
	//
	// Verification: pitch-up (τ_p=-1) → fin=[+1,+1,-1,-1] = ++-- ✓
	//               yaw-right(τ_y=-1) → fin=[-1,+1,+1,-1] = -++- ✓
	//               roll+    (τ_r=+1) → fin=[+1,+1,+1,+1] = ++++ ✓
	// ---------------------------------------------------------------
	const float max_defl = fmaxf(_param_max_defl.get(), 0.001f); // guard div-by-zero

	const float tau_pitch = delta_pitch / max_defl;
	const float tau_yaw   = delta_yaw   / max_defl;
	float       tau_roll  = delta_roll  / max_defl;

	// ---- Priority mixing: pitch/yaw FIRST, roll gets remaining ----
	// Roll budget: hard cap at 30% of fin range — prevents T2 saturation
	static constexpr float ROLL_BUDGET = 0.30f;
	tau_roll = math::constrain(tau_roll, -ROLL_BUDGET, ROLL_BUDGET);

	// Pitch+yaw per fin (protected — these values are guaranteed)
	const float py[4] = {
		-tau_pitch + tau_yaw,   // CS0=TR: -P+Y
		-tau_pitch - tau_yaw,   // CS1=BR: -P-Y
		 tau_pitch - tau_yaw,   // CS2=BL: +P-Y
		 tau_pitch + tau_yaw,   // CS3=TL: +P+Y
	};

	// Roll takes only what's left on each fin
	float fin[4];

	for (int i = 0; i < 4; i++) {
		const float room_up   =  1.0f - py[i];   // headroom toward +1
		const float room_down = -1.0f - py[i];   // headroom toward -1
		const float roll_i = math::constrain(tau_roll, room_down, room_up);
		fin[i] = py[i] + roll_i;
	}

	// ---------------------------------------------------------------
	// Publish torque setpoint → control_allocator handles XFin mixing
	// Only publish after launch to avoid 400 msg/s CAN traffic while
	// on the rail (fins were zeroed at arm transition by xqpower_can).
	// ---------------------------------------------------------------
	if (_launched) {
		vehicle_torque_setpoint_s torque{};
		torque.timestamp        = now;
		torque.timestamp_sample = sc.timestamp;
		torque.xyz[0]           = tau_roll;
		torque.xyz[1]           = tau_pitch;
		torque.xyz[2]           = tau_yaw;
		_vehicle_torque_setpoint_pub.publish(torque);

		// Publish zero thrust setpoint (rocket motor not controlled by PX4)
		vehicle_thrust_setpoint_s thrust{};
		thrust.timestamp        = now;
		thrust.timestamp_sample = sc.timestamp;
		thrust.xyz[0]           = 0.0f;
		thrust.xyz[1]           = 0.0f;
		thrust.xyz[2]           = 0.0f;
		_vehicle_thrust_setpoint_pub.publish(thrust);
	}

	// Publish directly to actuator_outputs_sim so simulator_mavlink can
	// forward fin commands to the HIL bridge via HIL_ACTUATOR_CONTROLS.
	// Always publish (even before launch) so the HITL bridge receives
	// fin commands from the first moment of flight.  xqpower_can already
	// ignores near-zero commands, and CAN bus load is negligible.
	{
		actuator_outputs_s ao{};
		ao.timestamp = now;
		ao.noutputs  = 4;
		ao.output[0] = fin[0];
		ao.output[1] = fin[1];
		ao.output[2] = fin[2];
		ao.output[3] = fin[3];
		_actuator_outputs_sim_pub.publish(ao);
	}

	// ---------------------------------------------------------------
	// Publish status
	// ---------------------------------------------------------------
	rocket_gnc_status_s status{};
	status.timestamp        = now;
	status.timestamp_sample = sc.timestamp;
	status.stage            = (uint8_t)stage;
	status.t_flight         = t;
	status.q_dyn            = q_dyn;
	status.q_kgf            = q_kgf;
	status.pitch_accel_cmd  = ac_pitch;
	status.yaw_accel_cmd    = ayc;
	status.yaw_los_deg      = yaw_los_deg;
	status.delta_roll       = delta_roll;
	status.delta_pitch      = delta_pitch;
	status.delta_yaw        = delta_yaw;
	status.fin1             = fin[0];
	status.fin2             = fin[1];
	status.fin3             = fin[2];
	status.fin4             = fin[3];
	status.altitude         = Ym;
	status.airspeed         = vm;
	status.rho              = air.rho;
	status.phi              = euler.phi();
	status.wx_filter        = _Wx_filter;
	status.du_roll          = _du_roll;
	status.out_integ_roll   = _out_integ_roll;

	// Read servo online mask published by xqpower_can in debug_array.data[12]
	debug_array_s dbg_srv{};

	if (_debug_array_sub.copy(&dbg_srv) && dbg_srv.id == 1) {
		status.servo_online_mask = (uint8_t)dbg_srv.data[12];
	}

	_rocket_gnc_status_pub.publish(status);

	perf_end(_loop_perf);
}

// ===================================================================
//  ModuleBase boilerplate
// ===================================================================

int RocketGNC::task_spawn(int argc, char *argv[])
{
	RocketGNC *instance = new RocketGNC();

	if (!instance) {
		PX4_ERR("alloc failed");
		return PX4_ERROR;
	}

	_object.store(instance);
	_task_id = task_id_is_work_queue;

	if (!instance->init()) {
		delete instance;
		_object.store(nullptr);
		_task_id = -1;
		return PX4_ERROR;
	}

	return PX4_OK;
}

int RocketGNC::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int RocketGNC::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Rocket M130 Guidance, Navigation & Control module.
Implements M130-specific autopilot, guidance (biased PN), and X-fin mixer.
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("rocket_gnc", "controller");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}

extern "C" __EXPORT int rocket_gnc_main(int argc, char *argv[])
{
	return RocketGNC::main(argc, argv);
}
