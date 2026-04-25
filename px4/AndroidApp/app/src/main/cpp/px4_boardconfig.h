/**
 * px4_boardconfig.h — Android Phone board configuration
 * Replaces the auto-generated kconfig header for our Android build
 * Values from Kconfig defaults
 */

#pragma once

// Platform
#define CONFIG_PLATFORM_POSIX 1
// CONFIG_BOARD_ROOT_PATH مُعرَّف في CMakeLists.txt كـ "." (مسار نسبي من chdir)
// لا نعيد تعريفه هنا لتجنب التعارض مع المسار الفعلي

// Modules enabled
#define CONFIG_MODULES_EKF2 1
#define CONFIG_MODULES_COMMANDER 1
#define CONFIG_MODULES_SENSORS 1
#define CONFIG_MODULES_MAVLINK 1
#define CONFIG_MODULES_NAVIGATOR 1
#define CONFIG_MODULES_LOGGER 1
#define CONFIG_MODULES_LAND_DETECTOR 1
#define CONFIG_MODULES_FLIGHT_MODE_MANAGER 1
#define CONFIG_MODULES_MANUAL_CONTROL 1
#define CONFIG_MODULES_MC_ATT_CONTROL 1
#define CONFIG_MODULES_MC_POS_CONTROL 1
#define CONFIG_MODULES_MC_RATE_CONTROL 1
#define CONFIG_MODULES_FW_ATT_CONTROL 1
#define CONFIG_MODULES_FW_RATE_CONTROL 1
#define CONFIG_MODULES_CONTROL_ALLOCATOR 1

// Board control — دعم الريبوت من QGC
#define CONFIG_BOARDCTL_RESET 1

// Disable simulation
#define CONFIG_MODULES_SIMULATION_SIMULATOR_SIH 0
#define CONFIG_MODULES_SIMULATION_GZ_BRIDGE 0

// ===== Work Queue Configuration (from Kconfig defaults) =====
#define CONFIG_WQ_RATE_CTRL_STACKSIZE   3150
#define CONFIG_WQ_RATE_CTRL_PRIORITY    0

#define CONFIG_WQ_SPI_STACKSIZE         2392
#define CONFIG_WQ_SPI0_PRIORITY         (-1)
#define CONFIG_WQ_SPI1_PRIORITY         (-2)
#define CONFIG_WQ_SPI2_PRIORITY         (-3)
#define CONFIG_WQ_SPI3_PRIORITY         (-4)
#define CONFIG_WQ_SPI4_PRIORITY         (-5)
#define CONFIG_WQ_SPI5_PRIORITY         (-6)
#define CONFIG_WQ_SPI6_PRIORITY         (-7)

#define CONFIG_WQ_I2C_STACKSIZE         2336
#define CONFIG_WQ_I2C0_PRIORITY         (-8)
#define CONFIG_WQ_I2C1_PRIORITY         (-9)
#define CONFIG_WQ_I2C2_PRIORITY         (-10)
#define CONFIG_WQ_I2C3_PRIORITY         (-11)
#define CONFIG_WQ_I2C4_PRIORITY         (-12)

#define CONFIG_WQ_NAV_AND_CONTROLLERS_STACKSIZE  2240
#define CONFIG_WQ_NAV_AND_CONTROLLERS_PRIORITY   (-13)

#define CONFIG_WQ_INS_STACKSIZE         6000
#define CONFIG_WQ_INS0_PRIORITY         (-14)
#define CONFIG_WQ_INS1_PRIORITY         (-15)
#define CONFIG_WQ_INS2_PRIORITY         (-16)
#define CONFIG_WQ_INS3_PRIORITY         (-17)

#define CONFIG_WQ_HP_DEFAULT_STACKSIZE  2800
#define CONFIG_WQ_HP_DEFAULT_PRIORITY   (-18)

#define CONFIG_WQ_UAVCAN_STACKSIZE      3624
#define CONFIG_WQ_UAVCAN_PRIORITY       (-19)

#define CONFIG_WQ_TTY_STACKSIZE         1728
#define CONFIG_WQ_TTY_S0_PRIORITY       (-21)
#define CONFIG_WQ_TTY_S1_PRIORITY       (-22)
#define CONFIG_WQ_TTY_S2_PRIORITY       (-23)
#define CONFIG_WQ_TTY_S3_PRIORITY       (-24)
#define CONFIG_WQ_TTY_S4_PRIORITY       (-25)
#define CONFIG_WQ_TTY_S5_PRIORITY       (-26)
#define CONFIG_WQ_TTY_S6_PRIORITY       (-27)
#define CONFIG_WQ_TTY_S7_PRIORITY       (-28)
#define CONFIG_WQ_TTY_S8_PRIORITY       (-29)
#define CONFIG_WQ_TTY_S9_PRIORITY       (-30)
#define CONFIG_WQ_TTY_ACM0_PRIORITY     (-31)
#define CONFIG_WQ_TTY_UNKNOWN_PRIORITY  (-32)

#define CONFIG_WQ_LP_DEFAULT_STACKSIZE  3500
#define CONFIG_WQ_LP_DEFAULT_PRIORITY   (-50)
