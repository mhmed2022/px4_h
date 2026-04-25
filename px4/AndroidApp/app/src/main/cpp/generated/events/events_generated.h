// auto-generated from events.h.jinja

#pragma once

#include <stdint.h>
#include <string.h>

namespace events
{
static constexpr int MAX_ARGUMENTS_SIZE = 25; ///< maximum number of bytes for all arguments

enum class LogLevel : uint8_t {
	Emergency = 0,
	Alert = 1,
	Critical = 2,
	Error = 3,
	Warning = 4,
	Notice = 5,
	Info = 6,
	Debug = 7,
	Protocol = 8,
	Disabled = 9,

	Count
};


enum class LogLevelInternal : uint8_t {
	Emergency = 0,
	Alert = 1,
	Critical = 2,
	Error = 3,
	Warning = 4,
	Notice = 5,
	Info = 6,
	Debug = 7,
	Protocol = 8,
	Disabled = 9,

	Count
};


using Log = LogLevel;
using LogInternal = LogLevelInternal;

struct LogLevels {
	LogLevels() {}
	LogLevels(Log external_level) : external(external_level), internal((LogInternal)external_level) {}
	LogLevels(Log external_level, LogInternal internal_level)
		: external(external_level), internal(internal_level) {}

	Log external{Log::Info};
	LogInternal internal{LogInternal::Info};
};

static inline LogInternal internalLogLevel(uint8_t log_levels) {
	return (LogInternal)(log_levels >> 4);
}

static inline Log externalLogLevel(uint8_t log_levels) {
	return (Log)(log_levels & 0xF);
}


namespace common // component id: 0
{
namespace enums
{

enum class calibration_type_t : uint16_t {
	accel = 1,
	mag = 2,
	gyro = 4,
	level = 8,
	airspeed = 16,
	rc = 32,
	_max = 32
};

static inline calibration_type_t operator|(calibration_type_t a, calibration_type_t b)
{
	return static_cast<calibration_type_t>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

static inline bool operator&(calibration_type_t a, calibration_type_t b)
{
	return static_cast<uint16_t>(a) & static_cast<uint16_t>(b);
}

enum class calibration_sides_t : uint8_t {
	tail_down = 1,
	nose_down = 2,
	left_side_down = 4,
	right_side_down = 8,
	upside_down = 16,
	down = 32,
	_max = 32
};

static inline calibration_sides_t operator|(calibration_sides_t a, calibration_sides_t b)
{
	return static_cast<calibration_sides_t>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

static inline bool operator&(calibration_sides_t a, calibration_sides_t b)
{
	return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

enum class calibration_action_t : uint8_t {
	already_completed = 0,
	next_orientation = 1,
	rotate = 2,
	hold_still = 3,
	_max = 3
};

enum class calibration_result_t : uint8_t {
	success = 0,
	failed = 1,
	aborted = 2,
	_max = 2
};

} // namespace enums
} // namespace common


namespace common // component id: 0
{

template<typename EventTypeT>
static inline EventTypeT create_cal_progress(const LogLevels &log_levels, uint8_t proto_ver, int8_t progress, common::enums::calibration_type_t calibration_type, common::enums::calibration_sides_t required_sides)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	EventTypeT event{};
	event.id = 1100;
	event.log_levels = ((uint8_t)log_levels.internal << 4) | (uint8_t)log_levels.external;
	memcpy(event.arguments+0, &proto_ver, sizeof(uint8_t));
	memcpy(event.arguments+1, &progress, sizeof(int8_t));
	memcpy(event.arguments+2, &calibration_type, sizeof(common::enums::calibration_type_t));
	memcpy(event.arguments+4, &required_sides, sizeof(common::enums::calibration_sides_t));
	return event;
}

template<typename EventTypeT>
static inline void decode_cal_progress(const EventTypeT &event, uint8_t &proto_ver, int8_t &progress, common::enums::calibration_type_t &calibration_type, common::enums::calibration_sides_t &required_sides)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	memcpy(&proto_ver, event.arguments+0, sizeof(uint8_t));
	memcpy(&progress, event.arguments+1, sizeof(int8_t));
	memcpy(&calibration_type, event.arguments+2, sizeof(common::enums::calibration_type_t));
	memcpy(&required_sides, event.arguments+4, sizeof(common::enums::calibration_sides_t));
}

template<typename EventTypeT>
static inline EventTypeT create_cal_orientation_detected(const LogLevels &log_levels, common::enums::calibration_sides_t orientation, common::enums::calibration_action_t action)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	EventTypeT event{};
	event.id = 1101;
	event.log_levels = ((uint8_t)log_levels.internal << 4) | (uint8_t)log_levels.external;
	memcpy(event.arguments+0, &orientation, sizeof(common::enums::calibration_sides_t));
	memcpy(event.arguments+1, &action, sizeof(common::enums::calibration_action_t));
	return event;
}

template<typename EventTypeT>
static inline void decode_cal_orientation_detected(const EventTypeT &event, common::enums::calibration_sides_t &orientation, common::enums::calibration_action_t &action)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	memcpy(&orientation, event.arguments+0, sizeof(common::enums::calibration_sides_t));
	memcpy(&action, event.arguments+1, sizeof(common::enums::calibration_action_t));
}

template<typename EventTypeT>
static inline EventTypeT create_cal_orientation_done(const LogLevels &log_levels, common::enums::calibration_sides_t orientation, common::enums::calibration_action_t action)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	EventTypeT event{};
	event.id = 1102;
	event.log_levels = ((uint8_t)log_levels.internal << 4) | (uint8_t)log_levels.external;
	memcpy(event.arguments+0, &orientation, sizeof(common::enums::calibration_sides_t));
	memcpy(event.arguments+1, &action, sizeof(common::enums::calibration_action_t));
	return event;
}

template<typename EventTypeT>
static inline void decode_cal_orientation_done(const EventTypeT &event, common::enums::calibration_sides_t &orientation, common::enums::calibration_action_t &action)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	memcpy(&orientation, event.arguments+0, sizeof(common::enums::calibration_sides_t));
	memcpy(&action, event.arguments+1, sizeof(common::enums::calibration_action_t));
}

template<typename EventTypeT>
static inline EventTypeT create_cal_done(const LogLevels &log_levels, common::enums::calibration_result_t result)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	EventTypeT event{};
	event.id = 1103;
	event.log_levels = ((uint8_t)log_levels.internal << 4) | (uint8_t)log_levels.external;
	memcpy(event.arguments+0, &result, sizeof(common::enums::calibration_result_t));
	return event;
}

template<typename EventTypeT>
static inline void decode_cal_done(const EventTypeT &event, common::enums::calibration_result_t &result)
{
	static_assert(MAX_ARGUMENTS_SIZE <= sizeof(EventTypeT::arguments), "Argument size mismatch");
	memcpy(&result, event.arguments+0, sizeof(common::enums::calibration_result_t));
}


enum class event_id_t : uint32_t {
	cal_progress = 1100,
	cal_orientation_detected = 1101,
	cal_orientation_done = 1102,
	cal_done = 1103,
};

} // namespace common


namespace px4 // component id: 1
{
namespace enums
{

enum class navigation_mode_group_t : uint32_t {
	manual = 1,
	altctl = 2,
	posctl = 4,
	mission = 8,
	loiter = 16,
	rtl = 32,
	acro = 1024,
	offboard = 16384,
	stab = 32768,
	takeoff = 131072,
	land = 262144,
	follow_target = 524288,
	precland = 1048576,
	orbit = 2097152,
	vtol_takeoff = 4194304,
	external1 = 8388608,
	external2 = 16777216,
	external3 = 33554432,
	external4 = 67108864,
	external5 = 134217728,
	external6 = 268435456,
	external7 = 536870912,
	external8 = 1073741824,
	_max = 1073741824
};

static inline navigation_mode_group_t operator|(navigation_mode_group_t a, navigation_mode_group_t b)
{
	return static_cast<navigation_mode_group_t>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

static inline bool operator&(navigation_mode_group_t a, navigation_mode_group_t b)
{
	return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
}

enum class health_component_t : uint32_t {
	none = 1,
	absolute_pressure = 2,
	differential_pressure = 4,
	gps = 8,
	optical_flow = 16,
	vision_position = 32,
	distance_sensor = 64,
	remote_control = 128,
	motors_escs = 256,
	utm = 512,
	logging = 1024,
	battery = 2048,
	communication_links = 4096,
	rate_controller = 8192,
	attitude_controller = 16384,
	position_controller = 32768,
	attitude_estimate = 65536,
	local_position_estimate = 131072,
	mission = 262144,
	avoidance = 524288,
	system = 1048576,
	camera = 2097152,
	gimbal = 4194304,
	payload = 8388608,
	global_position_estimate = 16777216,
	storage = 33554432,
	parachute = 67108864,
	magnetometer = 134217728,
	accel = 268435456,
	gyro = 536870912,
	open_drone_id = 1073741824,
	traffic_avoidance = 2147483648u,
	_max = 2147483648u
};

static inline health_component_t operator|(health_component_t a, health_component_t b)
{
	return static_cast<health_component_t>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

static inline bool operator&(health_component_t a, health_component_t b)
{
	return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
}

enum class sensor_type_t : uint8_t {
	accel = 0,
	gyro = 1,
	mag = 2,
};

enum class sensor_failover_reason_t : uint16_t {
	no_data = 1,
	stale_data = 2,
	timeout = 4,
	high_error_count = 8,
	high_error_density = 16,
	_max = 16
};

static inline sensor_failover_reason_t operator|(sensor_failover_reason_t a, sensor_failover_reason_t b)
{
	return static_cast<sensor_failover_reason_t>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

static inline bool operator&(sensor_failover_reason_t a, sensor_failover_reason_t b)
{
	return static_cast<uint16_t>(a) & static_cast<uint16_t>(b);
}

enum class arming_state_t : uint8_t {
	init = 0,
	standby = 1,
	armed = 2,
	standby_error = 3,
	shutdown = 4,
	inair_restore = 5,
};

enum class failsafe_cause_t : uint8_t {
	generic = 0,
	manual_control_loss = 1,
	gcs_connection_loss = 2,
	low_battery_level = 3,
	critical_battery_level = 4,
	emergency_battery_level = 5,
	low_remaining_flight_time = 6,
	_max = 6
};

enum class failsafe_action_t : uint8_t {
	none = 0,
	warn = 1,
	fallback_posctrl = 2,
	fallback_altctrl = 3,
	fallback_stabilized = 4,
	hold = 5,
	rtl = 6,
	land = 7,
	descend = 8,
	disarm = 9,
	terminate = 10,
	_max = 10
};

enum class arm_disarm_reason_t : uint8_t {
	stick_gesture = 1,
	rc_switch = 2,
	command_internal = 3,
	command_external = 4,
	mission_start = 5,
	landing = 6,
	preflight_inaction = 7,
	kill_switch = 8,
	rc_button = 13,
	failsafe = 14,
};

enum class navigation_mode_t : uint8_t {
	manual = 0,
	altctl = 1,
	posctl = 2,
	auto_mission = 3,
	auto_loiter = 4,
	auto_rtl = 5,
	acro = 6,
	offboard = 7,
	stab = 8,
	position_slow = 9,
	auto_takeoff = 10,
	auto_land = 11,
	auto_follow_target = 12,
	auto_precland = 13,
	orbit = 14,
	auto_vtol_takeoff = 15,
	external1 = 16,
	external2 = 17,
	external3 = 18,
	external4 = 19,
	external5 = 20,
	external6 = 21,
	external7 = 22,
	external8 = 23,
	altitude_cruise = 24,
	unknown = 255,
};

enum class battery_fault_reason_t : uint8_t {
	deep_discharge = 0,
	voltage_spikes = 1,
	cell_fail = 2,
	over_current = 3,
	over_temperature = 4,
	under_temperature = 5,
	incompatible_voltage = 6,
	incompatible_firmware = 7,
	incompatible_model = 8,
	hardware_fault = 9,
	failed_to_arm = 10,
	_max = 10
};

enum class esc_fault_reason_t : uint8_t {
	over_current = 0,
	over_voltage = 1,
	motor_over_temp = 2,
	over_rpm = 3,
	inconsistent_cmd = 4,
	motor_stuck = 5,
	failure_generic = 6,
	motor_warn_temp = 7,
	esc_warn_temp = 8,
	esc_over_temp = 9,
	_max = 9
};

enum class suggested_action_t : uint8_t {
	none = 0,
	land = 1,
	reduce_throttle = 2,
};

} // namespace enums
} // namespace px4


} // namespace events
