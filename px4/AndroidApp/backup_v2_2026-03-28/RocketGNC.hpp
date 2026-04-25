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
 * @file RocketGNC.hpp
 * Rocket M130 Guidance, Navigation & Control module.
 *
 * Implements M130AutopilotSystem + M130GuidanceSystem + XFinMixer
 * ported from 6DOF_v2_130/dynamics/m130_autopilot.py and m130_guidance.py
 */

#pragma once

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#include <drivers/drv_hrt.h>
#include <perf/perf_counter.h>
#include <matrix/math.hpp>

#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/vehicle_air_data.h>
#include <uORB/topics/vehicle_torque_setpoint.h>
#include <uORB/topics/vehicle_thrust_setpoint.h>
#include <uORB/topics/actuator_outputs.h>
#include <uORB/topics/rocket_gnc_status.h>
#include <uORB/topics/debug_array.h>
#include <systemlib/mavlink_log.h>

using namespace time_literals;
using matrix::Eulerf;
using matrix::Quatf;
using matrix::Matrix3f;
using matrix::Vector3f;

#define MODULE_NAME "rocket_gnc"

class RocketGNC : public ModuleBase<RocketGNC>, public ModuleParams,
	public px4::ScheduledWorkItem
{
public:
	RocketGNC();
	~RocketGNC() override;

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	bool init();

private:
	void Run() override;
	void parameters_update(bool force = false);

	// ---------------------------------------------------------------
	// Autopilot helpers
	// ---------------------------------------------------------------
	int  _determineStage(float t, float yaw_los_deg);
	void _pilotGain(int stage, float q_kgf,
	                float &K_w_pitch, float &K_w_yaw, float &K_w_roll,
	                float &K_p_integ_acc, float &K_i_integ_acc,
	                float &K_p_integ_roll, float &K_i_integ_roll,
	                float &out_integ_limit, float &apc_limit,
	                float &Acc_sat);
	float _calcSpeedScaler(float vm);
	float _altitudeAutopilot(float sp, float h_fb, float q_kgf,
	                         float ss, float dt, float t);
	float _apcAutopilot(float fz_fb, float q_fb, float ac_pitch,
	                    float q_kgf, float dt);

	// ---------------------------------------------------------------
	// Guidance helpers
	// ---------------------------------------------------------------
	float _yawComm(float Vzm, float Zm);
	float _yawComdd2(float Xm, float Zm, float Vxm, float Vzm, float t);
	float _pn2(float Xm, float Ym, float Vxm, float Vym, float Vzm,
	           float phi, const Quatf &q, float t, float apc_limit);

	// ---------------------------------------------------------------
	// uORB Subscriptions
	// ---------------------------------------------------------------
	uORB::Subscription _sensor_combined_sub{ORB_ID(sensor_combined)};
	// SITL (POSIX): read ground-truth from simulator_mavlink (bypasses EKF2).
	// HITL (NuttX): read from vehicle topics (HIL_STATE_QUATERNION → mavlink receiver).
#ifdef __PX4_NUTTX
	uORB::Subscription _vehicle_attitude_sub{ORB_ID(vehicle_attitude)};
	uORB::Subscription _vehicle_local_position_sub{ORB_ID(vehicle_local_position)};
#else
	uORB::Subscription _vehicle_attitude_sub{ORB_ID(vehicle_attitude_groundtruth)};
	uORB::Subscription _vehicle_local_position_sub{ORB_ID(vehicle_local_position_groundtruth)};
#endif
	uORB::Subscription _vehicle_air_data_sub{ORB_ID(vehicle_air_data)};
	uORB::Subscription _vehicle_status_sub{ORB_ID(vehicle_status)};
	uORB::Subscription _parameter_update_sub{ORB_ID(parameter_update)};
	uORB::Subscription _debug_array_sub{ORB_ID(debug_array)};  // servo online mask from xqpower_can

	// ---------------------------------------------------------------
	// uORB Publications
	// ---------------------------------------------------------------
	uORB::Publication<vehicle_torque_setpoint_s> _vehicle_torque_setpoint_pub{ORB_ID(vehicle_torque_setpoint)};
	uORB::Publication<vehicle_thrust_setpoint_s> _vehicle_thrust_setpoint_pub{ORB_ID(vehicle_thrust_setpoint)};
	uORB::Publication<actuator_outputs_s>        _actuator_outputs_sim_pub{ORB_ID(actuator_outputs_sim)};
	uORB::Publication<rocket_gnc_status_s>       _rocket_gnc_status_pub{ORB_ID(rocket_gnc_status)};

	// ---------------------------------------------------------------
	// State variables
	// ---------------------------------------------------------------
	hrt_abstime _time_launch{0};
	bool        _launched{false};
	bool        _hitl_mode{false};         // HITL mode: use velocity-based launch detection
	float       _launch_dv{0.0f};          // Delta-V accumulator for launch detection
	float       _prev_yaw_los_deg{0.0f};

	// Arming origin — position at moment of arming becomes (0,0,0)
	bool  _armed{false};
	float _arm_origin_x{0.0f};
	float _arm_origin_y{0.0f};
	float _arm_origin_z{0.0f};

	// Autopilot IIR filter states
	float _Wx_filter{0.0f};
	float _Wy_filter{0.0f};
	float _Wz_filter{0.0f};
	float _Ayb_filter{0.0f};
	float _Azb_filter{0.0f};

	// Autopilot diagnostics
	float _du_roll{0.0f};

	// Autopilot integrators
	float _out_integ_roll{0.0f};
	float _out_integ_yaw{0.0f};
	float _apc_accum{0.0f};

	// Altitude controller states
	float _alt_int_accum{0.0f};
	float _alt_prev_herror{0.0f};
	float _alt_ed_filt{0.0f};

	// Stage tracking
	bool  _stage1_completed{false};
	float _stage1_end_time{0.0f};

	// Guidance pn2 state
	bool  _pn2_initialized{false};
	float _pn2_t_old{0.0f};
	float _pn2_Bref{0.0f};
	float _pn2_b_int{0.0f};
	float _pn2_b1{0.0f};

	// Bearing rotation — NED → downrange/crossrange frame
	float _cos_bearing{1.0f};
	float _sin_bearing{0.0f};
	float _target_downrange{0.0f};   // horizontal range to target (m)

	// ---------------------------------------------------------------
	// Performance counters
	// ---------------------------------------------------------------
	perf_counter_t _loop_perf{perf_alloc(PC_ELAPSED, MODULE_NAME": cycle")};
	orb_advert_t   _mavlink_log_pub{nullptr};

	// ---------------------------------------------------------------
	// Parameters
	// ---------------------------------------------------------------
	DEFINE_PARAMETERS(
		(ParamFloat<px4::params::ROCKET_SET_ALT>)  _param_set_alt,
		(ParamFloat<px4::params::ROCKET_T_STG1>)   _param_t_stg1,
		(ParamFloat<px4::params::ROCKET_T_CTRL>)   _param_t_ctrl,
		(ParamFloat<px4::params::ROCKET_NPN>)       _param_npn,
		(ParamFloat<px4::params::ROCKET_IMP_ANG>)   _param_impact_ang,
		(ParamFloat<px4::params::ROCKET_TAU_PN1>)   _param_tau_pn1,
		(ParamFloat<px4::params::ROCKET_K_YAW>)     _param_k_yaw,
		(ParamFloat<px4::params::ROCKET_K_VZ>)      _param_k_vz,
		(ParamFloat<px4::params::ROCKET_AYC_LIM>)   _param_ayc_lim,
		(ParamFloat<px4::params::ROCKET_APC_LIM>)   _param_apc_lim,
		(ParamFloat<px4::params::ROCKET_XTRGT>)     _param_xtrgt,
		(ParamFloat<px4::params::ROCKET_YTRGT>)     _param_ytrgt,
		(ParamFloat<px4::params::ROCKET_ZTRGT>)     _param_ztrgt,
		(ParamFloat<px4::params::ROCKET_MAX_DEFL>)  _param_max_defl,
		(ParamInt<px4::params::ROCKET_GND_TEST>)    _param_gnd_test
	)
};
