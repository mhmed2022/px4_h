/**
 * board_config.h — Android Phone board configuration
 * Based on SITL board_config.h, adapted for Android
 */

#pragma once

#define BOARD_OVERRIDE_UUID "ANDROIDPHONE0000"
#define PX4_SOC_ARCH_ID     0

#define BOARD_HAS_POWER_CONTROL 0
#define PX4_NUMBER_I2C_BUSES 1
#define PX4_I2C_BUS_CLOCK_INIT {100000}
#define BOARD_NUMBER_BRICKS 0
#define BOARD_HAS_CONTROL_STATUS_LEDS 0

// RC serial port (not used on Android, but required by board_common.h)
#define RC_SERIAL_PORT ""

#include <system_config.h>
#include <px4_platform_common/board_common.h>
