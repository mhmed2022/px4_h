cmake_minimum_required(VERSION 3.22)
project(px4phone_native CXX C)

# PX4_ROOT يشير إلى مستودع PX4 (داخل cpp/)
# CMakeLists.txt يقع في: AndroidApp/app/src/main/cpp/
# PX4 يقع في:           AndroidApp/app/src/main/cpp/PX4-Autopilot/
set(PX4_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/PX4-Autopilot)

# ===== Python for auto-generation =====
find_program(PYTHON_EXECUTABLE NAMES python python3 HINTS
    "C:/Users/pc/AppData/Local/Programs/Python/Python310"
    ENV PATH
    NO_CMAKE_FIND_ROOT_PATH
)
if(NOT PYTHON_EXECUTABLE)
    message(FATAL_ERROR "Python not found! Required for uORB/MAVLink/params generation.")
endif()
message(STATUS "Python found: ${PYTHON_EXECUTABLE}")

# ===== Auto-generate uORB topic files from .msg =====
set(UORB_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated_uorb_topics)
set(UORB_MSG_DIR ${PX4_ROOT}/msg)
set(UORB_TEMPLATE_DIR ${PX4_ROOT}/Tools/msg/templates/uorb)
set(UORB_GEN_SCRIPT ${PX4_ROOT}/Tools/msg/px_generate_uorb_topic_files.py)

file(GLOB MSG_FILES ${UORB_MSG_DIR}/*.msg)
file(GLOB MSG_FILES_VERSIONED ${UORB_MSG_DIR}/versioned/*.msg)
list(APPEND MSG_FILES ${MSG_FILES_VERSIONED})

file(MAKE_DIRECTORY ${UORB_GEN_DIR})

# Generate headers
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${UORB_GEN_SCRIPT}
        --headers
        -f ${MSG_FILES}
        -i ${UORB_MSG_DIR} ${UORB_MSG_DIR}/versioned
        -e ${UORB_TEMPLATE_DIR}
        -o ${UORB_GEN_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE UORB_HDR_RESULT
)
if(NOT UORB_HDR_RESULT EQUAL 0)
    message(FATAL_ERROR "uORB header generation failed!")
endif()

# Generate sources
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${UORB_GEN_SCRIPT}
        --sources
        -f ${MSG_FILES}
        -i ${UORB_MSG_DIR} ${UORB_MSG_DIR}/versioned
        -e ${UORB_TEMPLATE_DIR}
        -o ${UORB_GEN_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE UORB_SRC_RESULT
)
if(NOT UORB_SRC_RESULT EQUAL 0)
    message(FATAL_ERROR "uORB source generation failed!")
endif()

message(STATUS "uORB topics generated in ${UORB_GEN_DIR}")

# Copy headers to uORB/topics/ subdirectory (PX4 includes as <uORB/topics/xxx.h>)
file(MAKE_DIRECTORY ${UORB_GEN_DIR}/uORB/topics)
file(GLOB UORB_GEN_HEADERS ${UORB_GEN_DIR}/*.h ${UORB_GEN_DIR}/*.hpp)
foreach(HDR ${UORB_GEN_HEADERS})
    get_filename_component(HDR_NAME ${HDR} NAME)
    configure_file(${HDR} ${UORB_GEN_DIR}/uORB/topics/${HDR_NAME} COPYONLY)
endforeach()
# Auto-generate uORBMessageFieldsGenerated (compressed field definitions)
# Step 1: Generate JSON from .msg files (includes msg/ + msg/versioned/)
set(UORB_JSON_DIR ${CMAKE_CURRENT_BINARY_DIR}/uorb_json)
file(MAKE_DIRECTORY ${UORB_JSON_DIR})
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${UORB_GEN_SCRIPT}
        --json -f ${MSG_FILES}
        -i ${UORB_MSG_DIR} ${UORB_MSG_DIR}/versioned
        -e ${UORB_TEMPLATE_DIR}
        -o ${UORB_JSON_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE UORB_JSON_RESULT
)
if(NOT UORB_JSON_RESULT EQUAL 0)
    message(FATAL_ERROR "uORB JSON generation failed!")
endif()

# Step 2: Generate compressed fields from JSON
file(GLOB UORB_JSON_FILES ${UORB_JSON_DIR}/*.json)
set(COMPRESSED_FIELDS_SCRIPT ${PX4_ROOT}/Tools/msg/px_generate_uorb_compressed_fields.py)
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${COMPRESSED_FIELDS_SCRIPT}
        -f ${UORB_JSON_FILES}
        --header-output-file ${UORB_GEN_DIR}/uORB/topics/uORBMessageFieldsGenerated.hpp
        --source-output-file ${UORB_GEN_DIR}/uORBMessageFieldsGenerated.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE UORB_COMPRESSED_RESULT
)
if(NOT UORB_COMPRESSED_RESULT EQUAL 0)
    message(FATAL_ERROR "uORB compressed fields generation failed!")
endif()
message(STATUS "uORB compressed fields auto-generated")

# ===== Auto-generate build_git_version.h =====
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

# Pin PX4 firmware version tag so AUTOPILOT_VERSION reports a valid version
# to MAVLink ground stations (e.g. QGroundControl). The embedded PX4 tree has
# no upstream tag, so `git describe` falls back to the commit SHA which fails
# version_tag_to_number() parsing and yields flight_sw_version = 0.
set(PX4_GIT_TAG "v1.17.0-alpha1")

execute_process(COMMAND git -C ${PX4_ROOT} rev-parse HEAD
    OUTPUT_VARIABLE PX4_GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
execute_process(COMMAND git -C ${PX4_ROOT} rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE PX4_GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
if(NOT PX4_GIT_VERSION)
    set(PX4_GIT_VERSION "0000000000000000")
endif()
if(NOT PX4_GIT_BRANCH)
    set(PX4_GIT_BRANCH "unknown")
endif()

string(SUBSTRING "${PX4_GIT_VERSION}" 0 16 PX4_GIT_VERSION_HEX)

file(WRITE ${GEN_DIR}/build_git_version.h
"#pragma once\n\
#define PX4_GIT_VERSION_STR      \"${PX4_GIT_VERSION}\"\n\
#define PX4_GIT_VERSION_BINARY    0x${PX4_GIT_VERSION_HEX}ULL\n\
#define PX4_GIT_TAG_STR          \"${PX4_GIT_TAG}\"\n\
#define PX4_GIT_BRANCH_STR       \"${PX4_GIT_BRANCH}\"\n\
#define PX4_GIT_BRANCH_NAME      \"${PX4_GIT_BRANCH}\"\n\
#define PX4_GIT_OEM_VERSION_STR  \"\"\n\
#define PX4_GIT_TAG_OR_BRANCH_NAME \"${PX4_GIT_BRANCH}\"\n\
#define MAVLINK_LIB_GIT_VERSION_STR    \"unknown\"\n\
#define MAVLINK_LIB_GIT_VERSION_BINARY  0x0000000000000000ULL\n\
#define ECL_LIB_GIT_VERSION_STR    \"unknown\"\n\
#define ECL_LIB_GIT_VERSION_BINARY  0x0000000000000000ULL\n"
)
message(STATUS "build_git_version.h generated: ${PX4_GIT_TAG}")

# ===== Auto-generate MAVLink headers =====
set(MAVLINK_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/mavlink_generated)
set(MAVGEN_SCRIPT ${PX4_ROOT}/src/modules/mavlink/mavlink/pymavlink/tools/mavgen.py)
set(MAVLINK_DEFS ${PX4_ROOT}/src/modules/mavlink/mavlink/message_definitions/v1.0)

# Generate development dialect (includes common + standard + minimal)
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${MAVGEN_SCRIPT}
        --lang=C --wire-protocol=2.0
        --output=${MAVLINK_GEN_DIR}/v2.0
        ${MAVLINK_DEFS}/development.xml
    RESULT_VARIABLE MAV_RESULT1
)
# Generate uAvionix dialect
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${MAVGEN_SCRIPT}
        --lang=C --wire-protocol=2.0
        --output=${MAVLINK_GEN_DIR}/v2.0
        ${MAVLINK_DEFS}/uAvionix.xml
    RESULT_VARIABLE MAV_RESULT2
)
if(NOT MAV_RESULT1 EQUAL 0 OR NOT MAV_RESULT2 EQUAL 0)
    message(FATAL_ERROR "MAVLink header generation failed!")
endif()
# Create mavlink/ subdirectory symlinks (PX4 includes as <mavlink/common/mavlink.h>)
file(MAKE_DIRECTORY ${MAVLINK_GEN_DIR}/v2.0/mavlink)
file(GLOB MAVLINK_DIALECTS ${MAVLINK_GEN_DIR}/v2.0/*)
foreach(DIALECT ${MAVLINK_DIALECTS})
    if(IS_DIRECTORY ${DIALECT})
        get_filename_component(DIALECT_NAME ${DIALECT} NAME)
        if(NOT ${DIALECT_NAME} STREQUAL "mavlink")
            file(COPY ${DIALECT}/ DESTINATION ${MAVLINK_GEN_DIR}/v2.0/mavlink/${DIALECT_NAME})
        endif()
    endif()
endforeach()
message(STATUS "MAVLink headers generated in ${MAVLINK_GEN_DIR}")

# ===== Force-include visibility.h for __EXPORT, __BEGIN_DECLS etc =====
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${PX4_ROOT}/src/include/visibility.h")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${PX4_ROOT}/src/include/visibility.h")

# ===== Android + PX4 POSIX definitions =====
add_definitions(
    -D__PX4_POSIX
    -D__PX4_LINUX
    -D__PX4_ANDROID
    -D__STDC_FORMAT_MACROS
    -DMODULE_NAME="px4"
    # Note: DEBUG_BUILD removed — it uses string-literal-concatenation log macros
    # that break when format string is a variable (Commander.cpp, estimatorCheck.cpp)

    # Board identity
    -DCONFIG_ARCH_BOARD_PX4_ANDROID=1
    -DCONFIG_NUM_MISSION_ITMES_SUPPORTED=1000
    -DPX4_BOARD_NAME=\"px4_android\"
    -DPX4_BOARD_LABEL=\"\"
    -DCONFIG_LOGGER_STACK_SIZE=6000

    # Storage root path — relative to chdir() in px4_jni.cpp
    -DCONFIG_BOARD_ROOT_PATH=\".\"

    # MAVLink UDP support (required for QGroundControl connection)
    -DMAVLINK_UDP=1

    # Disable NuttX-specific features
    -DPARAM_NO_ORB=0
    -DPARAM_NO_AUTOSAVE=0

    # === EKF2 sensor fusion (بدون هذه التعريفات EKF2 أعمى!) ===
    -DCONFIG_EKF2_BAROMETER=1
    -DCONFIG_EKF2_BARO_COMPENSATION=1
    -DCONFIG_EKF2_GNSS=1
    -DCONFIG_EKF2_GNSS_YAW=1
    -DCONFIG_EKF2_MAGNETOMETER=1
    -DCONFIG_EKF2_GRAVITY_FUSION=1
    -DCONFIG_EKF2_WIND=1
    -DCONFIG_EKF2_TERRAIN=1
    -DCONFIG_EKF2_AIRSPEED=1
    -DCONFIG_EKF2_SIDESLIP=1
    -DCONFIG_EKF2_DRAG_FUSION=1
    -DCONFIG_EKF2_AUXVEL=1
    -DCONFIG_EKF2_OPTICAL_FLOW=1
    -DCONFIG_EKF2_RANGE_FINDER=1
    -DCONFIG_EKF2_EXTERNAL_VISION=1
    -DCONFIG_EKF2_AUX_GLOBAL_POSITION=1

    # === Sensors module sub-modules (بدونها المعايرة لا تعمل!) ===
    -DCONFIG_SENSORS_VEHICLE_ACCELERATION=1
    -DCONFIG_SENSORS_VEHICLE_ANGULAR_VELOCITY=1
    -DCONFIG_SENSORS_VEHICLE_AIR_DATA=1
    -DCONFIG_SENSORS_VEHICLE_GPS_POSITION=1
    -DCONFIG_SENSORS_VEHICLE_MAGNETOMETER=1

    # === Heatshrink: static allocation (must match uORBMessageFields.hpp) ===
    -DHEATSHRINK_DYNAMIC_ALLOC=0

    # === Dataman: حفظ المهمات والـ geofence والـ rally points في ملف (وليس RAM) ===
    -DCONFIG_DATAMAN_PERSISTENT_STORAGE=1
)

# ===== Compiler flags =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# ===== Include paths =====
include_directories(
    # Our files first
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${UORB_GEN_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/parameters
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/events

    # PX4 source
    ${PX4_ROOT}/src
    ${PX4_ROOT}/src/include
    ${PX4_ROOT}/src/lib
    ${PX4_ROOT}/src/lib/matrix
    ${PX4_ROOT}/src/lib/slew_rate
    ${PX4_ROOT}/src/lib/heatshrink/heatshrink
    ${PX4_ROOT}/src/modules
    ${PX4_ROOT}/src/modules/mc_att_control/AttitudeControl
    ${PX4_ROOT}/src/lib/control_allocation/actuator_effectiveness
    ${PX4_ROOT}/src/lib/control_allocation/control_allocation
    ${PX4_ROOT}/src/modules/control_allocator/VehicleActuatorEffectiveness
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/mixer_module
    ${PX4_ROOT}/src/drivers

    # Platforms
    ${PX4_ROOT}/platforms
    ${PX4_ROOT}/platforms/posix/include
    ${PX4_ROOT}/platforms/common/include
    ${PX4_ROOT}/platforms/common/uORB
    ${PX4_ROOT}/platforms/common

    # POSIX generic architecture (for px4_arch/micro_hal.h)
    ${PX4_ROOT}/platforms/posix/src/px4/generic/generic/include
    # POSIX common includes (for px4_platform/micro_hal.h)
    ${PX4_ROOT}/platforms/posix/src/px4/common/include

    # MAVLink generated headers (auto-generated from XML definitions)
    ${MAVLINK_GEN_DIR}/v2.0
    ${MAVLINK_GEN_DIR}/v2.0/common
    ${MAVLINK_GEN_DIR}/v2.0/minimal
    ${MAVLINK_GEN_DIR}/v2.0/standard
    ${MAVLINK_GEN_DIR}/v2.0/development
    ${MAVLINK_GEN_DIR}/v2.0/uAvionix

    # Module-specific includes
    ${PX4_ROOT}/src/modules/mavlink
    ${PX4_ROOT}/src/modules/ekf2
    ${PX4_ROOT}/src/modules/ekf2/EKF
    ${PX4_ROOT}/src/modules/sensors
    ${PX4_ROOT}/src/modules/load_mon
    ${PX4_ROOT}/src/modules/simulation/simulator_mavlink
    # EKF derivation generated files (state.h etc)
    ${PX4_ROOT}/src/modules/ekf2/EKF/python

    # flight_mode_manager task subdirectories
    ${PX4_ROOT}/src/modules/flight_mode_manager
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/FlightTask
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Auto
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/AutoFollowTarget
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/AutoFollowTarget/follow_target_estimator
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/AltitudeCruise
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Descend
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Failsafe
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/ManualAcceleration
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/ManualAccelerationSlow
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/ManualAltitude
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/ManualAltitudeSmoothVel
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/ManualPosition
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Orbit
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Transition
    ${PX4_ROOT}/src/modules/flight_mode_manager/tasks/Utility

    # PositionControl sub-library
    ${PX4_ROOT}/src/modules/mc_pos_control/PositionControl
    # GotoControl sub-library
    ${PX4_ROOT}/src/modules/mc_pos_control/GotoControl

    # GeofenceBreachAvoidance
    ${PX4_ROOT}/src/modules/navigator/GeofenceBreachAvoidance

    # motion_planning
    ${PX4_ROOT}/src/lib/motion_planning

    # Takeoff
    ${PX4_ROOT}/src/modules/mc_pos_control/Takeoff

    # PX4 sensor drivers
    ${PX4_ROOT}/src/lib/drivers/accelerometer
    ${PX4_ROOT}/src/lib/drivers/gyroscope
    ${PX4_ROOT}/src/lib/drivers/magnetometer
    ${PX4_ROOT}/src/lib/drivers/device

    # Version
    ${PX4_ROOT}/src/lib/version

    # Timesync
    ${PX4_ROOT}/src/lib/timesync

    # Tunes
    ${PX4_ROOT}/src/lib/tunes

    # Navigator sub-libraries
    ${PX4_ROOT}/src/modules/navigator/MissionFeasibility

    # WeatherVane
    ${PX4_ROOT}/src/lib/weather_vane

    # Variable length ringbuffer
    ${PX4_ROOT}/src/lib/variable_length_ringbuffer

    # GPS UBX driver (u-blox binary protocol)
    ${PX4_ROOT}/src/drivers/gps
    ${PX4_ROOT}/src/drivers/gps/devices/src
)

# ===== مصادرنا (ملفات نكتبها نحن) =====
set(OUR_SOURCES
    px4_jni.cpp
    native_sensor_reader.cpp
    android_uorb_publishers.cpp
    mavlink_tcp_bridge.cpp
    gps_usb_ubx.cpp
    mavlink_pty_usb_bridge.cpp
    arduino_pty_usb_bridge.cpp
    arduino_output.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/apps.cpp
)

# ===== Generated uORB topic sources (auto-generated from .msg files) =====
file(GLOB UORB_GENERATED_SRCS ${UORB_GEN_DIR}/*.cpp)
list(APPEND OUR_SOURCES ${UORB_GENERATED_SRCS})
# uORBMessageFieldsGenerated (compressed field definitions — auto-generated)
list(APPEND OUR_SOURCES ${UORB_GEN_DIR}/uORBMessageFieldsGenerated.cpp)

# ===== مصادر PX4 =====
set(PX4_SOURCES "")

# ===== Platform / uORB =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/uORB/uORBManager.cpp
    ${PX4_ROOT}/platforms/common/uORB/uORBDeviceMaster.cpp
    ${PX4_ROOT}/platforms/common/uORB/uORBDeviceNode.cpp
    ${PX4_ROOT}/platforms/common/uORB/SubscriptionInterval.cpp
    ${PX4_ROOT}/platforms/common/uORB/uORB.cpp
    ${PX4_ROOT}/platforms/common/uORB/uORBUtils.cpp
    ${PX4_ROOT}/platforms/common/uORB/Subscription.cpp
    ${PX4_ROOT}/platforms/common/px4_log.cpp
    ${PX4_ROOT}/platforms/common/module.cpp
    ${PX4_ROOT}/platforms/common/px4_cli.cpp
    ${PX4_ROOT}/platforms/common/shutdown.cpp
    ${PX4_ROOT}/platforms/common/external_reset_lockout.cpp
    ${PX4_ROOT}/platforms/common/px4_log_history.cpp
)
# uORBMessageFields (needs heatshrink)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/uORB/uORBMessageFields.cpp
)

# ===== WorkQueue =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/px4_work_queue/WorkQueueManager.cpp
    ${PX4_ROOT}/platforms/common/px4_work_queue/WorkItem.cpp
    ${PX4_ROOT}/platforms/common/px4_work_queue/ScheduledWorkItem.cpp
)

# ===== Work queue (C files) =====
file(GLOB WORK_QUEUE_C_SRCS ${PX4_ROOT}/platforms/common/work_queue/*.c)
list(FILTER WORK_QUEUE_C_SRCS EXCLUDE REGEX ".*wqueue_test.*")
list(APPEND PX4_SOURCES ${WORK_QUEUE_C_SRCS})

# ===== POSIX platform =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/posix/src/px4/common/drv_hrt.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/tasks.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/server.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/client.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/sock_protocol.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/pxh.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/history.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_daemon/server_io.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_init.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_posix_impl.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/px4_sem.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/cpuload.cpp
    ${PX4_ROOT}/platforms/posix/src/px4/common/print_load.cpp
)

# ===== Parameters =====
# Note: parameters.cpp #includes parameters_common.cpp internally
# Do NOT add parameters_common.cpp or parameters_primary.cpp separately
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/parameters/parameters.cpp
    ${PX4_ROOT}/src/lib/parameters/autosave.cpp
    ${PX4_ROOT}/src/lib/parameters/atomic_transaction.cpp
)

# ===== EKF2 =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/ekf2/EKF2.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF2Selector.cpp
)
# EKF core files
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/ekf2/EKF/control.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/covariance.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/ekf.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/ekf_helper.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/estimator_interface.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/height_control.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/position_fusion.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/velocity_fusion.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/yaw_fusion.cpp
    # Sub-modules
    ${PX4_ROOT}/src/modules/ekf2/EKF/bias_estimator/bias_estimator.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/imu_down_sampler/imu_down_sampler.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/output_predictor/output_predictor.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/yaw_estimator/EKFGSF_yaw.cpp
)

# ===== Core Modules =====
# sensors (recursive to include vehicle_imu etc)
file(GLOB_RECURSE SENSORS_SRCS ${PX4_ROOT}/src/modules/sensors/*.cpp)
list(FILTER SENSORS_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER SENSORS_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(FILTER SENSORS_SRCS EXCLUDE REGEX ".*/tests?/.*")
list(FILTER SENSORS_SRCS EXCLUDE REGEX ".*/data_validator/.*")
list(APPEND PX4_SOURCES ${SENSORS_SRCS})

# commander
file(GLOB_RECURSE COMMANDER_SRCS ${PX4_ROOT}/src/modules/commander/*.cpp)
list(FILTER COMMANDER_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER COMMANDER_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(FILTER COMMANDER_SRCS EXCLUDE REGEX ".*Commander_tests.*")
list(FILTER COMMANDER_SRCS EXCLUDE REGEX ".*emscripten.*")
list(APPEND PX4_SOURCES ${COMMANDER_SRCS})

# navigator
file(GLOB NAVIGATOR_SRCS ${PX4_ROOT}/src/modules/navigator/*.cpp)
list(FILTER NAVIGATOR_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(APPEND PX4_SOURCES ${NAVIGATOR_SRCS})

# mavlink
file(GLOB MAVLINK_SRCS ${PX4_ROOT}/src/modules/mavlink/*.cpp)
list(FILTER MAVLINK_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER MAVLINK_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(FILTER MAVLINK_SRCS EXCLUDE REGEX ".*open_drone_id_translations\\.cpp$")
list(APPEND PX4_SOURCES ${MAVLINK_SRCS})

# logger
file(GLOB LOGGER_SRCS ${PX4_ROOT}/src/modules/logger/*.cpp)
list(FILTER LOGGER_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER LOGGER_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${LOGGER_SRCS})

# load_mon (CPU/RAM monitoring — مطلوب من commander)
list(APPEND PX4_SOURCES ${PX4_ROOT}/src/modules/load_mon/LoadMon.cpp)

# land_detector
file(GLOB LAND_DETECTOR_SRCS ${PX4_ROOT}/src/modules/land_detector/*.cpp)
list(FILTER LAND_DETECTOR_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(APPEND PX4_SOURCES ${LAND_DETECTOR_SRCS})

# flight_mode_manager
file(GLOB_RECURSE FMM_SRCS ${PX4_ROOT}/src/modules/flight_mode_manager/*.cpp)
list(FILTER FMM_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER FMM_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${FMM_SRCS})

# manual_control
file(GLOB MANUAL_CTRL_SRCS ${PX4_ROOT}/src/modules/manual_control/*.cpp)
list(FILTER MANUAL_CTRL_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER MANUAL_CTRL_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${MANUAL_CTRL_SRCS})

# ===== Controllers =====
file(GLOB MC_ATT_SRCS ${PX4_ROOT}/src/modules/mc_att_control/*.cpp)
file(GLOB MC_POS_SRCS ${PX4_ROOT}/src/modules/mc_pos_control/*.cpp)
file(GLOB MC_RATE_SRCS ${PX4_ROOT}/src/modules/mc_rate_control/*.cpp)
file(GLOB FW_ATT_SRCS ${PX4_ROOT}/src/modules/fw_att_control/*.cpp)
file(GLOB FW_RATE_SRCS ${PX4_ROOT}/src/modules/fw_rate_control/*.cpp)
file(GLOB CTRL_ALLOC_SRCS ${PX4_ROOT}/src/modules/control_allocator/*.cpp)
list(FILTER MC_ATT_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER MC_POS_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER MC_RATE_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER FW_ATT_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER FW_RATE_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER CTRL_ALLOC_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(APPEND PX4_SOURCES
    ${MC_ATT_SRCS} ${MC_POS_SRCS} ${MC_RATE_SRCS}
    ${FW_ATT_SRCS} ${FW_RATE_SRCS} ${CTRL_ALLOC_SRCS}
)

# ===== Libraries =====
# geo
file(GLOB GEO_SRCS ${PX4_ROOT}/src/lib/geo/*.cpp)
list(FILTER GEO_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER GEO_SRCS EXCLUDE REGEX ".*test_.*\\.cpp$")
list(APPEND PX4_SOURCES ${GEO_SRCS})

# mixer_module
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/mixer_module/mixer_module.cpp
)

# cdev
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/cdev/CDev.cpp
)

# perf_counter
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/perf/perf_counter.cpp
)

# sensor_calibration
file(GLOB SENSOR_CAL_SRCS ${PX4_ROOT}/src/lib/sensor_calibration/*.cpp)
list(APPEND PX4_SOURCES ${SENSOR_CAL_SRCS})

# systemlib
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/systemlib/mavlink_log.cpp
)

# hysteresis
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/hysteresis/hysteresis.cpp
)

# lat_lon_alt
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/lat_lon_alt/lat_lon_alt.cpp
)

# heatshrink
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/heatshrink/heatshrink/heatshrink_decoder.c
)

# data_validator
file(GLOB DATA_VALIDATOR_SRCS ${PX4_ROOT}/src/modules/sensors/data_validator/*.cpp)
list(FILTER DATA_VALIDATOR_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER DATA_VALIDATOR_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${DATA_VALIDATOR_SRCS})

# EKF2 aid_sources
file(GLOB_RECURSE EKF2_AID_SRCS
    ${PX4_ROOT}/src/modules/ekf2/EKF/aid_sources/*.cpp
)
list(APPEND PX4_SOURCES ${EKF2_AID_SRCS})

# EKF2 wind + terrain
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/ekf2/EKF/wind.cpp
    ${PX4_ROOT}/src/modules/ekf2/EKF/terrain_control.cpp
)

# events
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/events/events.cpp
)

# WorkQueue
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/px4_work_queue/WorkQueue.cpp
)

# cdev POSIX platform
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/cdev/posix/cdev_platform.cpp
)

# circuit_breaker
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/circuit_breaker/circuit_breaker.cpp
)

# rotation conversion
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/conversion/rotation.cpp
)

# atmosphere
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/atmosphere/atmosphere.cpp
)

# dataman_client
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/dataman_client/DatamanClient.cpp
)

# rtl
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/rtl/rtl_time_estimator.cpp
)

# mixer_module actuator_test
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/mixer_module/actuator_test.cpp
)

# ===== System commands (MAVLink console) =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/systemcmds/actuator_test/actuator_test.cpp
    ${PX4_ROOT}/src/systemcmds/param/param.cpp
    ${PX4_ROOT}/src/systemcmds/ver/ver.cpp
    ${PX4_ROOT}/src/systemcmds/top/top.cpp
    ${PX4_ROOT}/src/systemcmds/perf/perf.cpp
    ${PX4_ROOT}/src/systemcmds/topic_listener/listener_main.cpp
    ${PX4_ROOT}/src/systemcmds/reboot/reboot.cpp
)

# AttitudeControl
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mc_att_control/AttitudeControl/AttitudeControl.cpp
)

# StickYaw
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/stick_yaw/StickYaw.cpp
)

# tinybson
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/tinybson/tinybson.cpp
)

# px4_getopt
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/px4_getopt.c
)

# VehicleActuatorEffectiveness
file(GLOB VEHICLE_AE_SRCS ${PX4_ROOT}/src/modules/control_allocator/VehicleActuatorEffectiveness/*.cpp)
list(FILTER VEHICLE_AE_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER VEHICLE_AE_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${VEHICLE_AE_SRCS})

# control_allocation library
file(GLOB CTRL_ALLOC_LIB_SRCS ${PX4_ROOT}/src/lib/control_allocation/control_allocation/*.cpp)
list(FILTER CTRL_ALLOC_LIB_SRCS EXCLUDE REGEX ".*_test\\.cpp$")
list(FILTER CTRL_ALLOC_LIB_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${CTRL_ALLOC_LIB_SRCS})

# actuator_effectiveness library
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/control_allocation/actuator_effectiveness/ActuatorEffectiveness.cpp
)

# collision_prevention
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/collision_prevention/CollisionPrevention.cpp
)

# rate_control + PID
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/rate_control/rate_control.cpp
    ${PX4_ROOT}/src/lib/rate_control/gain_compression.cpp
    ${PX4_ROOT}/src/lib/pid/PID.cpp
)

# sticks
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/sticks/Sticks.cpp
)

# motion_planning
file(GLOB MOTION_PLANNING_SRCS ${PX4_ROOT}/src/lib/motion_planning/*.cpp)
list(FILTER MOTION_PLANNING_SRCS EXCLUDE REGEX ".*Test\\.cpp$")
list(APPEND PX4_SOURCES ${MOTION_PLANNING_SRCS})

# PositionControl
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mc_pos_control/PositionControl/PositionControl.cpp
    ${PX4_ROOT}/src/modules/mc_pos_control/PositionControl/ControlMath.cpp
)

# GeofenceBreachAvoidance
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/navigator/GeofenceBreachAvoidance/geofence_breach_avoidance.cpp
)

# ObstacleMath
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/collision_prevention/ObstacleMath.cpp
)

# mavlink.c (defines mavlink_system global)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mavlink/mavlink.c
)

# crc32 (POSIX version)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/posix/src/px4/common/lib_crc32.c
)

# GotoControl
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mc_pos_control/GotoControl/GotoControl.cpp
)

# PX4 sensor drivers
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/drivers/accelerometer/PX4Accelerometer.cpp
    ${PX4_ROOT}/src/lib/drivers/gyroscope/PX4Gyroscope.cpp
    ${PX4_ROOT}/src/lib/drivers/magnetometer/PX4Magnetometer.cpp
)

# Takeoff handling
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mc_pos_control/Takeoff/Takeoff.cpp
)

# Timesync
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/timesync/Timesync.cpp
)

# Tunes
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/tunes/tunes.cpp
)

# VariableLengthRingbuffer
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/variable_length_ringbuffer/VariableLengthRingbuffer.cpp
)

# WeatherVane
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/weather_vane/WeatherVane.cpp
)

# Version info
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/version/version.c
)

# FeasibilityChecker (navigator)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/navigator/MissionFeasibility/FeasibilityChecker.cpp
)

# FlightTasks_generated (auto-generated from templates)
set(FT_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${FT_GEN_DIR})
execute_process(
    COMMAND ${PYTHON_EXECUTABLE}
        ${PX4_ROOT}/src/modules/flight_mode_manager/generate_flight_tasks.py
        -t Auto AutoFollowTarget Descend Failsafe ManualAcceleration
           ManualAccelerationSlow ManualAltitude ManualAltitudeSmoothVel
           ManualPosition Orbit Transition AltitudeCruise
        -i ${PX4_ROOT}/src/modules/flight_mode_manager/Templates
        -o ${FT_GEN_DIR}
        -f FlightTasks_generated.cpp FlightTasks_generated.hpp
    RESULT_VARIABLE FT_RESULT
)
if(NOT FT_RESULT EQUAL 0)
    message(FATAL_ERROR "FlightTasks generation failed!")
endif()
list(APPEND OUR_SOURCES
    ${FT_GEN_DIR}/FlightTasks_generated.cpp
)

# Ringbuffer
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/ringbuffer/Ringbuffer.cpp
)

# Tunes default definitions
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/tunes/default_tunes.cpp
)

# Board identity (POSIX)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/platforms/common/board_identity.c
)

# GPS UBX driver (u-blox binary protocol — used by gps_usb_ubx.cpp)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/drivers/gps/devices/src/ubx.cpp
    ${PX4_ROOT}/src/drivers/gps/devices/src/gps_helper.cpp
    ${PX4_ROOT}/src/drivers/gps/devices/src/rtcm.cpp
    ${PX4_ROOT}/src/drivers/gps/devices/src/crc.cpp
)

# World magnetic model
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/world_magnetic_model/geo_mag_declination.cpp
)

# Open Drone ID translations
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/mavlink/open_drone_id_translations.cpp
)

# Parameter translation/migration
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/lib/parameters/param_translation.cpp
)

# uorb system command
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/systemcmds/uorb/uorb.cpp
)

# dataman
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/dataman/dataman.cpp
)

# pwm_out_sim — HIL PWM simulation (publishes actuator_outputs_sim for simulator_mavlink)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/simulation/pwm_out_sim/PWMSim.cpp
)

# simulator_mavlink — HIL bridge (receives HIL_SENSOR, sends HIL_ACTUATOR_CONTROLS)
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/modules/simulation/simulator_mavlink/SimulatorMavlink.cpp
)

# ===== libusb 1.0 (USB Host — للسيرفوهات عبر USB-C OTG) =====
set(LIBUSB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libusb_src)
set(LIBUSB_SOURCES
    ${LIBUSB_ROOT}/libusb/core.c
    ${LIBUSB_ROOT}/libusb/descriptor.c
    ${LIBUSB_ROOT}/libusb/hotplug.c
    ${LIBUSB_ROOT}/libusb/io.c
    ${LIBUSB_ROOT}/libusb/sync.c
    ${LIBUSB_ROOT}/libusb/strerror.c
    ${LIBUSB_ROOT}/libusb/os/linux_usbfs.c
    ${LIBUSB_ROOT}/libusb/os/events_posix.c
    ${LIBUSB_ROOT}/libusb/os/threads_posix.c
    ${LIBUSB_ROOT}/libusb/os/linux_netlink.c
)
add_library(usb1.0 STATIC ${LIBUSB_SOURCES})
target_include_directories(usb1.0 PUBLIC
    ${LIBUSB_ROOT}/libusb
    ${LIBUSB_ROOT}/libusb/os
)
target_include_directories(usb1.0 PRIVATE
    ${LIBUSB_ROOT}/android
)
target_compile_options(usb1.0 PRIVATE -fvisibility=hidden -pthread -Wno-implicit-function-declaration)
target_compile_definitions(usb1.0 PRIVATE -D_GNU_SOURCE=1)
target_link_libraries(usb1.0 log)

# ===== Build =====
add_library(px4phone_native SHARED ${OUR_SOURCES} ${PX4_SOURCES})

# ===== Compiler flags =====
target_compile_options(px4phone_native PRIVATE
    # Suppressed warnings — only for third-party PX4 sources that we cannot fix.
    # First-party narrowing / format / sign-compare warnings are ENABLED
    # (suppression removed in PR-3b after PR-5 fixed the underlying code).
    -Wno-deprecated-declarations
    -Wno-address-of-packed-member
    -Wno-unused-variable
    -Wno-unused-function
    -Wno-unused-but-set-variable
    -Wno-missing-field-initializers
    -Wno-reorder
    -Wno-implicit-fallthrough
    -Wno-tautological-constant-out-of-range-compare
    -D_FORTIFY_SOURCE=2
    -fno-exceptions
    -fno-rtti
    -O2
)

target_link_libraries(px4phone_native
    log           # __android_log_print
    android       # ASensorManager, ALooper
    atomic        # std::atomic
    m             # math
    z             # zlib
    usb1.0        # libusb — USB Host للسيرفوهات
)
