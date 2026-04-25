
/**
 * The IQUART driver's baud rate
 *
 * The baud rate (in bits per second) used by the serial port connected with IQUART communication
 * 
 *
 * @group Vertiq IO
 * @reboot_required True
 */
PARAM_DEFINE_INT32(VTQ_BAUD, 115200);

/**
 * The number of Vertiq IFCI parameters to use
 *
 * The total number of IFCI control variables being used across all connected modules
 * 
 *
 * @group Vertiq IO
 * @min 0
 * @max 16
 * @reboot_required True
 */
PARAM_DEFINE_INT32(VTQ_NUM_CVS, 0);

/**
 * The triggered behavior sent to the motors on PX4 disarm
 *
 * The behavior triggered when the flight controller disarms. You have the option to trigger your motors' disarm behaviors, set all motors to coast,
 * or set a predefined throttle setpoint
 * 
 *
 * @group Vertiq IO
 * @value 0 Send Explicit Disarm
 * @value 1 Coast Motors
 * @value 2 Set Predefined Velocity Setpoint
 */
PARAM_DEFINE_INT32(VTQ_DISARM_TRIG, 0);

/**
 * Velocity sent when DISARM_TRIGGER is Set Predefined Velocity Setpoint
 *
 * This is the velocity that will be sent to all motors when PX4 is disarmed and DISARM_TRIGGER is Set Predefined Velocity Setpoint
 *
 * @group Vertiq IO
 * @min 0
 * @max 100
 */
PARAM_DEFINE_INT32(VTQ_DISARM_VELO, 0);

/**
 * The triggered behavior on PX4 arm
 *
 * The behavior triggered when the flight controller arms. You have the option to use your motors' arming behaviors, or to force all of your motors to arm
 * 
 *
 * @group Vertiq IO
 * @value 0 Use Motor Arm Behavior
 * @value 1 Send Explicit Arm Command
 */
PARAM_DEFINE_INT32(VTQ_ARM_BEHAVE, 0);

/**
 * The Module ID of the module you would like to communicate with
 *
 * This is the value used as the target module ID of all configuration parameters (not operational parameters). The Vertiq module with the
 * module ID matching this value will react to all get and set requests from PX4. Any Vertiq client made with dynamic object IDs should
 * use this value to instantiate itself.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_INT32(VTQ_TRGT_MOD_ID, 0);

/**
 * Reinitialize the target module's values into the PX4 parameters
 *
 * Setting this value to true will reinitialize PX4's IQUART connected parameters to the value stored on the currently targeted motor.
 * This is especially useful if your flight controller powered on before your connected modules
 * 
 *
 * @group Vertiq IO
 * @boolean
 */
PARAM_DEFINE_INT32(VTQ_REDO_READ, 0);

/**
 * Module Param - The module's Throttle Control Value Index
 *
 * This represents the Control Value Index where the targeted module will look for throttle commands
 * 
 *
 * @group Vertiq IO
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(VTQ_THROTTLE_CVI, 0);

/**
 * Module Param - The module's control mechanism
 *
 * PWM Mode: Commands a fraction of battery voltage. This changes as the battery voltage changes.
 * This is the least safe mode because the upper throttle limit is determined by the battery voltage.
 * Voltage Mode: Commands a voltage. The motor will behave the same way throughout the life of a battery, assuming the commanded voltage is less than the battery voltage.
 * You must set the MAX_VOLTS parameter.
 * Velocity Mode: Closed-loop, commands a velocity. The controller will adjust the applied voltage so that the motor spins at the commanded velocity.
 * This mode has faster reaction times. Only use this if you know the properties of your propeller. You must set the MAX_VELOCITY parameter.
 * 
 *
 * @group Vertiq IO
 * @value 0 PWM
 * @value 1 Voltage
 * @value 2 Velocity
 */
PARAM_DEFINE_INT32(VTQ_CONTROL_MODE, 0);

/**
 * Module Param - Maximum velocity when CONTROL_MODE is set to Velocity
 *
 * Only relevant in Velocity Mode. This is the velocity the controller will command at full throttle.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_MAX_VELOCITY, 0);

/**
 * Module Param - Maximum voltage when CONTROL_MODE is set to Voltage
 *
 * Only relevant in Voltage Mode. This is the voltage the controller will command at full throttle.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_MAX_VOLTS, 0);

/**
 * Module Param - The direction that the module should spin
 *
 * Set the targeted motor's spinning direction (clockwise vs. counter clockwise) and flight mode (2D non-reversible vs. 3D reversible)
 * 
 *
 * @group Vertiq IO
 * @value 0 Unconfigured
 * @value 1 3D Counter Clockwise
 * @value 2 3D Clockwise
 * @value 3 2D Counter Clockwise
 * @value 4 2D Clockwise
 */
PARAM_DEFINE_INT32(VTQ_MOTOR_DIR, 0);

/**
 * Module Param - If the flight controller uses 2D or 3D communication
 *
 * The FC and the ESC must agree upon the meaning of the signal coming out of the ESC. When FCs are in 3D mode
 * they re-map negative signals. This parameter keeps the FC and ESC in agreement.
 * 
 *
 * @group Vertiq IO
 * @value 0 2D
 * @value 1 3D
 */
PARAM_DEFINE_INT32(VTQ_FC_DIR, 0);

/**
 * Module Param - The encoder angle at which theta is zero
 *
 * The encoder angle at which theta is zero. Adjust this number to change the location of 0 phase when pulsing.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_ZERO_ANGLE, 0);

/**
 * Module Param - The minimum velocity required to allow pulsing
 *
 * This is the velocity at which pulsing is allowed. Any velocity between VELOCITY_CUTOFF and -VELOCITY_CUTOFF will not pulse.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_VELO_CUTOFF, 0);

/**
 * Module Param - Offsets pulse angle to allow for mechanical properties
 *
 * This offsets where the pulse starts around the motor to allow for propeller mechanical properties.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_TQUE_OFF_ANG, 0);

/**
 * Module Param - 0 = Supply Voltage Mode, 1 = Voltage Limit Mode
 *
 * Supply Voltage Mode means that the maximum voltage applied to pulsing is the supplied voltage. Voltage Limit Mode
 * indicates that PULSE_VOLT_LIM is the maximum allowed voltage to apply towards pulsing.
 * 
 *
 * @group Vertiq IO
 * @value 0 Supply Voltage Mode
 * @value 1 Voltage Limit Mode
 */
PARAM_DEFINE_INT32(VTQ_PULSE_V_MODE, 0);

/**
 * Module Param - Max pulsing voltage limit when in Voltage Limit Mode
 *
 * This sets the max pulsing voltage limit when in Voltage Limit Mode.
 * 
 *
 * @group Vertiq IO
 */
PARAM_DEFINE_FLOAT(VTQ_PULSE_V_LIM, 0);

/**
 * Module Param - CVI for the X rectangular coordinate
 *
 * This represents the Control Value Index where the targeted module will look for the X rectangular coordinate.
 * 
 *
 * @group Vertiq IO
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(VTQ_X_CVI, 0);

/**
 * Module Param - CVI for the Y rectangular coordinate
 *
 * This represents the Control Value Index where the targeted module will look for the Y rectangular coordinate.
 * 
 *
 * @group Vertiq IO
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(VTQ_Y_CVI, 0);

/**
 * Module IDs [0, 31] that you would like to request telemetry from
 *
 * The module IDs [0, 31] that should be asked for telemetry. The data received from these IDs will be published via the esc_status topic.
 * 
 *
 * @group Vertiq IO
 * @bit 0 Module ID 0
 * @bit 1 Module ID 1
 * @bit 2 Module ID 2
 * @bit 3 Module ID 3
 * @bit 4 Module ID 4
 * @bit 5 Module ID 5
 * @bit 6 Module ID 6
 * @bit 7 Module ID 7
 * @bit 8 Module ID 8
 * @bit 9 Module ID 9
 * @bit 10 Module ID 10
 * @bit 11 Module ID 11
 * @bit 12 Module ID 12
 * @bit 13 Module ID 13
 * @bit 14 Module ID 14
 * @bit 15 Module ID 15
 * @bit 16 Module ID 16
 * @bit 17 Module ID 17
 * @bit 18 Module ID 18
 * @bit 19 Module ID 19
 * @bit 20 Module ID 20
 * @bit 21 Module ID 21
 * @bit 22 Module ID 22
 * @bit 23 Module ID 23
 * @bit 24 Module ID 24
 * @bit 25 Module ID 25
 * @bit 26 Module ID 26
 * @bit 27 Module ID 27
 * @bit 28 Module ID 28
 * @bit 29 Module ID 29
 * @bit 30 Module ID 30
 * @bit 31 Module ID 31
 * @min 0
 * @max 4294967295
 * @reboot_required True
 */
PARAM_DEFINE_INT32(VTQ_TELEM_IDS_1, 0);

/**
 * Module IDs [32, 62] that you would like to request telemetry from
 *
 * The module IDs [32, 62] that should be asked for telemetry. The data received from these IDs will be published via the esc_status topic.
 * 
 *
 * @group Vertiq IO
 * @bit 0 Module ID 32
 * @bit 1 Module ID 33
 * @bit 2 Module ID 34
 * @bit 3 Module ID 35
 * @bit 4 Module ID 36
 * @bit 5 Module ID 37
 * @bit 6 Module ID 38
 * @bit 7 Module ID 39
 * @bit 8 Module ID 40
 * @bit 9 Module ID 41
 * @bit 10 Module ID 42
 * @bit 11 Module ID 43
 * @bit 12 Module ID 44
 * @bit 13 Module ID 45
 * @bit 14 Module ID 46
 * @bit 15 Module ID 47
 * @bit 16 Module ID 48
 * @bit 17 Module ID 49
 * @bit 18 Module ID 50
 * @bit 19 Module ID 51
 * @bit 20 Module ID 52
 * @bit 21 Module ID 53
 * @bit 22 Module ID 54
 * @bit 23 Module ID 55
 * @bit 24 Module ID 56
 * @bit 25 Module ID 57
 * @bit 26 Module ID 58
 * @bit 27 Module ID 59
 * @bit 28 Module ID 60
 * @bit 29 Module ID 61
 * @bit 30 Module ID 62
 * @min 0
 * @max 2147483647
 * @reboot_required True
 */
PARAM_DEFINE_INT32(VTQ_TELEM_IDS_2, 0);

/**
 * Vertiq IO CVI 0 Output Function
 *
 * Select what should be output on Vertiq IO CVI 0.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC0, 0);

/**
 * Vertiq IO CVI 1 Output Function
 *
 * Select what should be output on Vertiq IO CVI 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC1, 0);

/**
 * Vertiq IO CVI 2 Output Function
 *
 * Select what should be output on Vertiq IO CVI 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC2, 0);

/**
 * Vertiq IO CVI 3 Output Function
 *
 * Select what should be output on Vertiq IO CVI 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC3, 0);

/**
 * Vertiq IO CVI 4 Output Function
 *
 * Select what should be output on Vertiq IO CVI 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC4, 0);

/**
 * Vertiq IO CVI 5 Output Function
 *
 * Select what should be output on Vertiq IO CVI 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC5, 0);

/**
 * Vertiq IO CVI 6 Output Function
 *
 * Select what should be output on Vertiq IO CVI 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC6, 0);

/**
 * Vertiq IO CVI 7 Output Function
 *
 * Select what should be output on Vertiq IO CVI 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC7, 0);

/**
 * Vertiq IO CVI 8 Output Function
 *
 * Select what should be output on Vertiq IO CVI 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC8, 0);

/**
 * Vertiq IO CVI 9 Output Function
 *
 * Select what should be output on Vertiq IO CVI 9.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC9, 0);

/**
 * Vertiq IO CVI 10 Output Function
 *
 * Select what should be output on Vertiq IO CVI 10.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC10, 0);

/**
 * Vertiq IO CVI 11 Output Function
 *
 * Select what should be output on Vertiq IO CVI 11.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC11, 0);

/**
 * Vertiq IO CVI 12 Output Function
 *
 * Select what should be output on Vertiq IO CVI 12.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC12, 0);

/**
 * Vertiq IO CVI 13 Output Function
 *
 * Select what should be output on Vertiq IO CVI 13.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC13, 0);

/**
 * Vertiq IO CVI 14 Output Function
 *
 * Select what should be output on Vertiq IO CVI 14.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC14, 0);

/**
 * Vertiq IO CVI 15 Output Function
 *
 * Select what should be output on Vertiq IO CVI 15.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VTQ_IO_FUNC15, 0);

/**
 * Vertiq IO CVI 0 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS0, 0);

/**
 * Vertiq IO CVI 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS1, 0);

/**
 * Vertiq IO CVI 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS2, 0);

/**
 * Vertiq IO CVI 3 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS3, 0);

/**
 * Vertiq IO CVI 4 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS4, 0);

/**
 * Vertiq IO CVI 5 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS5, 0);

/**
 * Vertiq IO CVI 6 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS6, 0);

/**
 * Vertiq IO CVI 7 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS7, 0);

/**
 * Vertiq IO CVI 8 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS8, 0);

/**
 * Vertiq IO CVI 9 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS9, 0);

/**
 * Vertiq IO CVI 10 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS10, 0);

/**
 * Vertiq IO CVI 11 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS11, 0);

/**
 * Vertiq IO CVI 12 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS12, 0);

/**
 * Vertiq IO CVI 13 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS13, 0);

/**
 * Vertiq IO CVI 14 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS14, 0);

/**
 * Vertiq IO CVI 15 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_DIS15, 0);

/**
 * Vertiq IO CVI 0 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN0, 0);

/**
 * Vertiq IO CVI 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN1, 0);

/**
 * Vertiq IO CVI 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN2, 0);

/**
 * Vertiq IO CVI 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN3, 0);

/**
 * Vertiq IO CVI 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN4, 0);

/**
 * Vertiq IO CVI 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN5, 0);

/**
 * Vertiq IO CVI 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN6, 0);

/**
 * Vertiq IO CVI 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN7, 0);

/**
 * Vertiq IO CVI 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN8, 0);

/**
 * Vertiq IO CVI 9 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN9, 0);

/**
 * Vertiq IO CVI 10 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN10, 0);

/**
 * Vertiq IO CVI 11 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN11, 0);

/**
 * Vertiq IO CVI 12 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN12, 0);

/**
 * Vertiq IO CVI 13 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN13, 0);

/**
 * Vertiq IO CVI 14 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN14, 0);

/**
 * Vertiq IO CVI 15 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MIN15, 0);

/**
 * Vertiq IO CVI 0 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX0, 65535);

/**
 * Vertiq IO CVI 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX1, 65535);

/**
 * Vertiq IO CVI 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX2, 65535);

/**
 * Vertiq IO CVI 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX3, 65535);

/**
 * Vertiq IO CVI 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX4, 65535);

/**
 * Vertiq IO CVI 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX5, 65535);

/**
 * Vertiq IO CVI 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX6, 65535);

/**
 * Vertiq IO CVI 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX7, 65535);

/**
 * Vertiq IO CVI 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX8, 65535);

/**
 * Vertiq IO CVI 9 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX9, 65535);

/**
 * Vertiq IO CVI 10 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX10, 65535);

/**
 * Vertiq IO CVI 11 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX11, 65535);

/**
 * Vertiq IO CVI 12 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX12, 65535);

/**
 * Vertiq IO CVI 13 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX13, 65535);

/**
 * Vertiq IO CVI 14 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX14, 65535);

/**
 * Vertiq IO CVI 15 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(VTQ_IO_MAX15, 65535);

/**
 * Vertiq IO CVI 0 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC0).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL0, -1);

/**
 * Vertiq IO CVI 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL1, -1);

/**
 * Vertiq IO CVI 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL2, -1);

/**
 * Vertiq IO CVI 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL3, -1);

/**
 * Vertiq IO CVI 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL4, -1);

/**
 * Vertiq IO CVI 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL5, -1);

/**
 * Vertiq IO CVI 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL6, -1);

/**
 * Vertiq IO CVI 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL7, -1);

/**
 * Vertiq IO CVI 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL8, -1);

/**
 * Vertiq IO CVI 9 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC9).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL9, -1);

/**
 * Vertiq IO CVI 10 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC10).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL10, -1);

/**
 * Vertiq IO CVI 11 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC11).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL11, -1);

/**
 * Vertiq IO CVI 12 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC12).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL12, -1);

/**
 * Vertiq IO CVI 13 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC13).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL13, -1);

/**
 * Vertiq IO CVI 14 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC14).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL14, -1);

/**
 * Vertiq IO CVI 15 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see VTQ_IO_FUNC15).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 500
 */
PARAM_DEFINE_INT32(VTQ_IO_FAIL15, -1);

/**
 * Reverse Output Range for Vertiq IO
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit -1 Vertiq IO CVI 0
 * @bit 0 Vertiq IO CVI 1
 * @bit 1 Vertiq IO CVI 2
 * @bit 2 Vertiq IO CVI 3
 * @bit 3 Vertiq IO CVI 4
 * @bit 4 Vertiq IO CVI 5
 * @bit 5 Vertiq IO CVI 6
 * @bit 6 Vertiq IO CVI 7
 * @bit 7 Vertiq IO CVI 8
 * @bit 8 Vertiq IO CVI 9
 * @bit 9 Vertiq IO CVI 10
 * @bit 10 Vertiq IO CVI 11
 * @bit 11 Vertiq IO CVI 12
 * @bit 12 Vertiq IO CVI 13
 * @bit 13 Vertiq IO CVI 14
 * @bit 14 Vertiq IO CVI 15
 * @min 0
 * @max 32767
 */
PARAM_DEFINE_INT32(VTQ_IO_REV, 0);

/**
 * Custom configuration for ModalAI drones
 *
 * This can be set to indicate that drone behavior
 * needs to be changed to match a custom setting
 * 
 *
 * @group ModalAI Custom Configuration
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MODALAI_CONFIG, 0);

/**
 * VOXL ESC Output ESC 1 Output Function
 *
 * Select what should be output on VOXL ESC Output ESC 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL_ESC_FUNC1, 0);

/**
 * VOXL ESC Output ESC 2 Output Function
 *
 * Select what should be output on VOXL ESC Output ESC 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL_ESC_FUNC2, 0);

/**
 * VOXL ESC Output ESC 3 Output Function
 *
 * Select what should be output on VOXL ESC Output ESC 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL_ESC_FUNC3, 0);

/**
 * VOXL ESC Output ESC 4 Output Function
 *
 * Select what should be output on VOXL ESC Output ESC 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL_ESC_FUNC4, 0);

/**
 * Reverse Output Range for VOXL ESC Output
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 VOXL ESC Output ESC 1
 * @bit 1 VOXL ESC Output ESC 2
 * @bit 2 VOXL ESC Output ESC 3
 * @bit 3 VOXL ESC Output ESC 4
 * @min 0
 * @max 15
 */
PARAM_DEFINE_INT32(VOXL_ESC_REV, 0);

/**
 * Enable ADS7953
 *
 * Enable the driver for the ADS7953 board
 * 
 *
 * @group ADC
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(ADC_ADS7953_EN, 0);

/**
 * Applied reference Voltage.
 *
 * The voltage applied to the ADS7953 board as reference
 * 
 *
 * @group ADC
 * @decimal 2
 * @increment 0.01
 * @min 2.0
 * @max 3.0
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(ADC_ADS7953_REFV, 2.5);

/**
 * Enable TLA2528
 *
 * Enable the driver for the TLA2528
 * 
 *
 * @group ADC
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(ADC_TLA2528_EN, 0);

/**
 * Applied reference Voltage.
 *
 * The voltage applied to the TLA2528 board as reference
 * 
 *
 * @group ADC
 * @decimal 2
 * @increment 0.01
 * @min 2.0
 * @max 3.0
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(ADC_TLA2528_REFV, 2.5);

/**
 * UAVCANv1 ESC 1 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC1, 0);

/**
 * UAVCANv1 ESC 2 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC2, 0);

/**
 * UAVCANv1 ESC 3 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC3, 0);

/**
 * UAVCANv1 ESC 4 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC4, 0);

/**
 * UAVCANv1 ESC 5 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC5, 0);

/**
 * UAVCANv1 ESC 6 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC6, 0);

/**
 * UAVCANv1 ESC 7 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC7, 0);

/**
 * UAVCANv1 ESC 8 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC8, 0);

/**
 * UAVCANv1 ESC 9 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 9.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC9, 0);

/**
 * UAVCANv1 ESC 10 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 10.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC10, 0);

/**
 * UAVCANv1 ESC 11 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 11.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC11, 0);

/**
 * UAVCANv1 ESC 12 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 12.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC12, 0);

/**
 * UAVCANv1 ESC 13 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 13.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC13, 0);

/**
 * UAVCANv1 ESC 14 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 14.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC14, 0);

/**
 * UAVCANv1 ESC 15 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 15.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC15, 0);

/**
 * UAVCANv1 ESC 16 Output Function
 *
 * Select what should be output on UAVCANv1 ESC 16.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FUNC16, 0);

/**
 * UAVCANv1 ESC 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN1, 1);

/**
 * UAVCANv1 ESC 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN2, 1);

/**
 * UAVCANv1 ESC 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN3, 1);

/**
 * UAVCANv1 ESC 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN4, 1);

/**
 * UAVCANv1 ESC 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN5, 1);

/**
 * UAVCANv1 ESC 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN6, 1);

/**
 * UAVCANv1 ESC 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN7, 1);

/**
 * UAVCANv1 ESC 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN8, 1);

/**
 * UAVCANv1 ESC 9 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN9, 1);

/**
 * UAVCANv1 ESC 10 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN10, 1);

/**
 * UAVCANv1 ESC 11 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN11, 1);

/**
 * UAVCANv1 ESC 12 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN12, 1);

/**
 * UAVCANv1 ESC 13 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN13, 1);

/**
 * UAVCANv1 ESC 14 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN14, 1);

/**
 * UAVCANv1 ESC 15 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN15, 1);

/**
 * UAVCANv1 ESC 16 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MIN16, 1);

/**
 * UAVCANv1 ESC 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX1, 8191);

/**
 * UAVCANv1 ESC 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX2, 8191);

/**
 * UAVCANv1 ESC 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX3, 8191);

/**
 * UAVCANv1 ESC 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX4, 8191);

/**
 * UAVCANv1 ESC 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX5, 8191);

/**
 * UAVCANv1 ESC 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX6, 8191);

/**
 * UAVCANv1 ESC 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX7, 8191);

/**
 * UAVCANv1 ESC 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX8, 8191);

/**
 * UAVCANv1 ESC 9 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX9, 8191);

/**
 * UAVCANv1 ESC 10 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX10, 8191);

/**
 * UAVCANv1 ESC 11 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX11, 8191);

/**
 * UAVCANv1 ESC 12 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX12, 8191);

/**
 * UAVCANv1 ESC 13 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX13, 8191);

/**
 * UAVCANv1 ESC 14 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX14, 8191);

/**
 * UAVCANv1 ESC 15 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX15, 8191);

/**
 * UAVCANv1 ESC 16 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_MAX16, 8191);

/**
 * UAVCANv1 ESC 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL1, -1);

/**
 * UAVCANv1 ESC 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL2, -1);

/**
 * UAVCANv1 ESC 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL3, -1);

/**
 * UAVCANv1 ESC 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL4, -1);

/**
 * UAVCANv1 ESC 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL5, -1);

/**
 * UAVCANv1 ESC 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL6, -1);

/**
 * UAVCANv1 ESC 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL7, -1);

/**
 * UAVCANv1 ESC 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL8, -1);

/**
 * UAVCANv1 ESC 9 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC9).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL9, -1);

/**
 * UAVCANv1 ESC 10 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC10).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL10, -1);

/**
 * UAVCANv1 ESC 11 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC11).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL11, -1);

/**
 * UAVCANv1 ESC 12 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC12).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL12, -1);

/**
 * UAVCANv1 ESC 13 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC13).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL13, -1);

/**
 * UAVCANv1 ESC 14 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC14).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL14, -1);

/**
 * UAVCANv1 ESC 15 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC15).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL15, -1);

/**
 * UAVCANv1 ESC 16 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UCAN1_ESC_FUNC16).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UCAN1_ESC_FAIL16, -1);

/**
 * Reverse Output Range for UAVCANv1
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 UAVCANv1 ESC 1
 * @bit 1 UAVCANv1 ESC 2
 * @bit 2 UAVCANv1 ESC 3
 * @bit 3 UAVCANv1 ESC 4
 * @bit 4 UAVCANv1 ESC 5
 * @bit 5 UAVCANv1 ESC 6
 * @bit 6 UAVCANv1 ESC 7
 * @bit 7 UAVCANv1 ESC 8
 * @bit 8 UAVCANv1 ESC 9
 * @bit 9 UAVCANv1 ESC 10
 * @bit 10 UAVCANv1 ESC 11
 * @bit 11 UAVCANv1 ESC 12
 * @bit 12 UAVCANv1 ESC 13
 * @bit 13 UAVCANv1 ESC 14
 * @bit 14 UAVCANv1 ESC 15
 * @bit 15 UAVCANv1 ESC 16
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(UCAN1_ESC_REV, 0);

/**
 * Distance Sensor Rotation
 *
 * Distance Sensor Rotation as MAV_SENSOR_ORIENTATION enum
 * 
 *
 * @group Sensors
 * @value 25 ROTATION_DOWNWARD_FACING
 * @value 24 ROTATION_UPWARD_FACING
 * @value 4 ROTATION_BACKWARD_FACING
 * @value 0 ROTATION_FORWARD_FACING
 * @value 6 ROTATION_LEFT_FACING
 * @value 2 ROTATION_RIGHT_FACING
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SENS_CM8JL65_R_0, 25);

/**
 * Lightware GRF lidar update rate.
 *
 * The Lightware GRF distance sensor can increase the update rate to enable greater resolution.
 * 
 *
 * @group Sensors
 * @value 1 1 Hz
 * @value 2 2 Hz
 * @value 3 4 Hz
 * @value 4 5 Hz
 * @value 5 10 Hz
 * @value 6 20 Hz
 * @value 7 30 Hz
 * @value 8 40 Hz
 * @value 9 50 Hz
 * @reboot_required True
 */
PARAM_DEFINE_INT32(GRF_RATE_CFG, 4);

/**
 * GRF Sensor model
 *
 * GRF Sensor Model used to distinush between the GRF250 and GRF500 since both have different max distance range.
 * 
 *
 * @group Sensors
 * @value 0 disable
 * @value 1 GRF250
 * @value 2 GRF500
 * @reboot_required True
 */
PARAM_DEFINE_INT32(GRF_SENS_MODEL, 0);

/**
 * Update rate in Hz
 *
 * The SF45 sets the update rate in Hz to allow greater resolution
 * 
 *
 * @group Sensors
 * @value 1 50hz
 * @value 2 100hz
 * @value 3 200hz
 * @value 4 400hz
 * @value 5 500hz
 * @value 6 625hz
 * @value 7 1000hz
 * @value 8 1250hz
 * @value 9 1538hz
 * @value 10 2000hz
 * @value 11 2500hz
 * @value 12 5000hz
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SF45_UPDATE_CFG, 5);

/**
 * Orientation upright or facing downward
 *
 * The SF45 mounted facing upward or downward on the frame
 * 
 *
 * @group Sensors
 * @value 24 Rotation upward
 * @value 25 Rotation downward
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SF45_ORIENT_CFG, 24);

/**
 * Sensor facing forward or backward
 *
 * The usb port on the sensor indicates 180deg, opposite usb is forward facing
 * 
 *
 * @group Sensors
 * @value 0 Rotation forward
 * @value 2 Rotation right
 * @value 4 Rotation backward
 * @value 6 Rotation left
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SF45_YAW_CFG, 0);

/**
 * Hardware Model
 *
 * Models differ in range and FoV.
 *
 * @group Sensors
 * @value 1 TFMINI
 * @value 2 ISTRA24
 * @value 3 ISTRA24_100m
 * @min 1
 * @max 3
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SENS_TFMINI_HW, 1);

/**
 * Main stream used during automatic configuration
 *
 * The stream the autopilot sets up on the receiver to output the main data.
 * 
 * Set this to another value if the default stream is already used for another purpose.
 * 
 *
 * @group Septentrio
 * @min 1
 * @max 10
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_STREAM_MAIN, 1);

/**
 * Logging stream used during automatic configuration
 *
 * The stream the autopilot sets up on the receiver to output the logging data.
 * 
 * Set this to another value if the default stream is already used for another purpose.
 * 
 *
 * @group Septentrio
 * @min 1
 * @max 10
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_STREAM_LOG, 2);

/**
 * Output frequency of main SBF blocks
 *
 * The output frequency of the main SBF blocks needed for PVT information.
 * 
 *
 * @group Septentrio
 * @value 0 5 Hz
 * @value 1 10 Hz
 * @value 2 20 Hz
 * @value 3 25 Hz
 * @min 0
 * @max 3
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_OUTP_HZ, 1);

/**
 * Heading/Yaw offset for dual antenna GPS
 *
 * Heading offset angle for dual antenna GPS setups that support heading estimation.
 * 
 * Set this to 0 if the antennas are parallel to the forward-facing direction
 * of the vehicle and the rover antenna is in front.
 * 
 * The offset angle increases clockwise.
 * 
 * Set this to 90 if the rover antenna is placed on the
 * right side of the vehicle and the moving base antenna is on the left side.
 * 
 *
 * @group Septentrio
 * @decimal 3
 * @min -360
 * @max 360
 * @unit deg
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(SEP_YAW_OFFS, 0);

/**
 * Enable sat info
 *
 * Enable publication of satellite info (ORB_ID(satellite_info)) if possible.
 * 
 *
 * @group Septentrio
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_SAT_INFO, 0);

/**
 * Pitch offset for dual antenna GPS
 *
 * Vertical offsets can be compensated for by adjusting the Pitch offset.
 * 
 * Note that this can be interpreted as the "roll" angle in case the antennas are aligned along the perpendicular axis.
 * This occurs in situations where the two antenna ARPs may not be exactly at the same height in the vehicle reference frame.
 * Since pitch is defined as the right-handed rotation about the vehicle Y axis,
 * a situation where the main antenna is mounted lower than the aux antenna (assuming the default antenna setup) will result in a positive pitch.
 * 
 *
 * @group Septentrio
 * @decimal 3
 * @min -90
 * @max 90
 * @unit deg
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(SEP_PITCH_OFFS, 0);

/**
 * Log GPS communication data
 *
 * Log raw communication between the driver and connected receivers.
 * For example, "To receiver" will log all commands and corrections sent by the driver to the receiver.
 * 
 *
 * @group Septentrio
 * @value 0 Disabled
 * @value 1 From receiver
 * @value 2 To receiver
 * @value 3 Both
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(SEP_DUMP_COMM, 0);

/**
 * Toggle automatic receiver configuration
 *
 * By default, the receiver is automatically configured. Sometimes it may be used for multiple purposes.
 * If the offered parameters aren't sufficient, this parameter can be disabled to have full control of the receiver configuration.
 * A good way to use this is to enable automatic configuration, let the receiver be configured, and then disable it to make manual adjustments.
 * 
 *
 * @group Septentrio
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_AUTO_CONFIG, 1);

/**
 * Usage of different constellations
 *
 * Choice of which constellations the receiver should use for PVT computation.
 * 
 * When this is 0, the constellation usage isn't changed.
 * 
 *
 * @group Septentrio
 * @bit 0 GPS
 * @bit 1 GLONASS
 * @bit 2 Galileo
 * @bit 3 SBAS
 * @bit 4 BeiDou
 * @min 0
 * @max 31
 * @min 0
 * @max 63
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_CONST_USAGE, 0);

/**
 * Logging frequency for the receiver
 *
 * Select the frequency at which the connected receiver should log data to its internal storage.
 * 
 *
 * @group Septentrio
 * @value 0 Disabled
 * @value 1 0.1 Hz
 * @value 2 0.2 Hz
 * @value 3 0.5 Hz
 * @value 4 1 Hz
 * @value 5 2 Hz
 * @value 6 5 Hz
 * @value 7 10 Hz
 * @value 8 20 Hz
 * @value 9 25 Hz
 * @value 10 50 Hz
 * @min 0
 * @max 10
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_LOG_HZ, 0);

/**
 * Logging level for the receiver
 *
 * Select the level of detail that needs to be logged by the receiver.
 * 
 *
 * @group Septentrio
 * @value 0 Lite
 * @value 1 Basic
 * @value 2 Default
 * @value 3 Full
 * @min 0
 * @max 3
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_LOG_LEVEL, 2);

/**
 * Whether to overwrite or add to existing logging
 *
 * When the receiver is already set up to log data, this decides whether extra logged data should be added or overwrite existing data.
 * 
 *
 * @group Septentrio
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_LOG_FORCE, 0);

/**
 * Setup and expected use of the hardware
 *
 * Setup and expected use of the hardware.
 * 
 * - Default: Use two receivers as completely separate instances.
 * - Moving base: Use two receivers in a rover & moving base setup for heading.
 * 
 *
 * @group Septentrio
 * @value 0 Default
 * @value 1 Moving base
 * @min 0
 * @max 1
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SEP_HARDW_SETUP, 0);

/**
 * The ID of the IMU controlled by heater 1
 *
 * Specifies the sensor device ID (DEVID) that this heater instance controls.
 * -1 disables this heater instance.
 * If set to 0, auto-select is only supported when HEATER_NUM == 1. On boards with multiple heater outputs,
 * a valid DEVID must be configured for each heater to ensure a 1:1 mapping between heater output and IMU.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_INT32(HEATER1_IMU_ID, 0);

/**
 * The ID of the IMU controlled by heater 2
 *
 * Specifies the sensor device ID (DEVID) that this heater instance controls.
 * -1 disables this heater instance.
 * If set to 0, auto-select is only supported when HEATER_NUM == 1. On boards with multiple heater outputs,
 * a valid DEVID must be configured for each heater to ensure a 1:1 mapping between heater output and IMU.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_INT32(HEATER2_IMU_ID, 0);

/**
 * The ID of the IMU controlled by heater 3
 *
 * Specifies the sensor device ID (DEVID) that this heater instance controls.
 * -1 disables this heater instance.
 * If set to 0, auto-select is only supported when HEATER_NUM == 1. On boards with multiple heater outputs,
 * a valid DEVID must be configured for each heater to ensure a 1:1 mapping between heater output and IMU.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_INT32(HEATER3_IMU_ID, 0);

/**
 * Target temperature for heater 1
 *
 * Specify the target stable temperature (in degrees Celsius) for the IMU.
 * It is generally recommended to set this between 40°C and 60°C,
 * which must be higher than the maximum ambient temperature.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 85.0
 * @unit celcius
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(HEATER1_TEMP, 55.0);

/**
 * Target temperature for heater 2
 *
 * Specify the target stable temperature (in degrees Celsius) for the IMU.
 * It is generally recommended to set this between 40°C and 60°C,
 * which must be higher than the maximum ambient temperature.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 85.0
 * @unit celcius
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(HEATER2_TEMP, 55.0);

/**
 * Target temperature for heater 3
 *
 * Specify the target stable temperature (in degrees Celsius) for the IMU.
 * It is generally recommended to set this between 40°C and 60°C,
 * which must be higher than the maximum ambient temperature.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 85.0
 * @unit celcius
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(HEATER3_TEMP, 55.0);

/**
 * IMU heater controller 1 feedforward value
 *
 * Used to predict the baseline power consumption required to maintain temperature,
 * helping to reduce adjustment time.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit %
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER1_TEMP_FF, 0.05);

/**
 * IMU heater controller 2 feedforward value
 *
 * Used to predict the baseline power consumption required to maintain temperature,
 * helping to reduce adjustment time.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit %
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER2_TEMP_FF, 0.05);

/**
 * IMU heater controller 3 feedforward value
 *
 * Used to predict the baseline power consumption required to maintain temperature,
 * helping to reduce adjustment time.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit %
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER3_TEMP_FF, 0.05);

/**
 * IMU heater controller 1 integrator gain value
 *
 * Integral gain is used to eliminate steady-state error,
 * ensuring that the temperature ultimately reaches the setpoint target.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER1_TEMP_I, 0.025);

/**
 * IMU heater controller 2 integrator gain value
 *
 * Integral gain is used to eliminate steady-state error,
 * ensuring that the temperature ultimately reaches the setpoint target.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER2_TEMP_I, 0.025);

/**
 * IMU heater controller 3 integrator gain value
 *
 * Integral gain is used to eliminate steady-state error,
 * ensuring that the temperature ultimately reaches the setpoint target.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 1.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER3_TEMP_I, 0.025);

/**
 * IMU heater controller 1 proportional gain value
 *
 * The proportional gain determines how quickly the controller responds to temperature deviations.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 2.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER1_TEMP_P, 1.0);

/**
 * IMU heater controller 2 proportional gain value
 *
 * The proportional gain determines how quickly the controller responds to temperature deviations.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 2.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER2_TEMP_P, 1.0);

/**
 * IMU heater controller 3 proportional gain value
 *
 * The proportional gain determines how quickly the controller responds to temperature deviations.
 * 
 *
 * @group Sensors
 * @decimal 3
 * @min 0
 * @max 2.0
 * @unit us/C
 * @reboot_required False
 */
PARAM_DEFINE_FLOAT(HEATER3_TEMP_P, 1.0);

/**
 * InertialLabs INS sensor mode configuration
 *
 * Configures whether the driver outputs only raw sensor output (the default),
 * or additionally supplies INS data such as position and velocity estimates.
 * 
 *
 * @group Sensors
 * @value 0 Sensors Only (default)
 * @value 1 INS
 * @category System
 */
PARAM_DEFINE_INT32(ILABS_MODE, 0);

/**
 * MicroStrain device mode
 *
 * Sensor mode publishes raw IMU data to be used by EKF2. INS data from the device is published to the external INS topics.
 * INS mode publishes the INS data to the vehicle topics to be used for navigation.
 * 
 *
 * @group Sensors
 * @value 0 Sensor Mode
 * @value 1 INS Mode
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_MODE, 0);

/**
 * MicroStrain IMU data rate
 *
 * Accelerometer and Gyroscope data rate (Hz).
 * Valid rates: 0 or any factor of 1000.
 * 
 *
 * @group Sensors
 * @min 0
 * @max 1000
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_IMU_RATE_HZ, 500);

/**
 * MicroStrain magnetometer data rate
 *
 * Magnetometer data rate (Hz).
 * Valid rates: 0 or any factor of 1000.
 * 
 *
 * @group Sensors
 * @min 0
 * @max 1000
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_MAG_RATE_HZ, 50);

/**
 * MicroStrain barometer data rate
 *
 * Barometer data rate (Hz).
 * Valid rates: 0 or any factor of 1000.
 * 
 *
 * @group Sensors
 * @min 0
 * @max 1000
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_BARO_RATE_HZ, 50);

/**
 * MicroStrain EKF data rate
 *
 * The rate at which the INS data is published (Hz).
 * Valid rates: 0 or any factor of 1000.
 * 
 *
 * @group Sensors
 * @min 0
 * @max 1000
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_FILT_RATE_HZ, 250);

/**
 * MicroStrain GNSS data rate
 *
 * GNSS receiver 1 and 2 data rate (Hz).
 * Valid rates: 0, 1 or 5.
 * 
 *
 * @group Sensors
 * @min 0
 * @max 5
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_GNSS_RATE_HZ, 5);

/**
 * MicroStrain heading alignment type
 *
 * Select the source of heading alignment.
 * 
 *
 * @group Sensors
 * @bit 0 Dual-antenna GNSS
 * @bit 1 GNSS kinematic (requires motion, e.g. a GNSS velocity)
 * @bit 2 Magnetometer
 * @bit 3 External Heading (first valid external heading will be used to initialize the filter)
 * @min 0
 * @max 15
 * @min 1
 * @max 15
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_ALIGNMENT, 2);

/**
 * MicroStrain GNSS aiding source control
 *
 * Select the source of gnss aiding (GNSS/INS).
 * 
 *
 * @group Sensors
 * @value 1 All internal receivers
 * @value 2 External GNSS messages
 * @value 3 GNSS receiver 1 only
 * @value 4 GNSS receiver 2 only
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_GNSS_AID_SRC, 1);

/**
 * Enable MicroStrain internal magnetometer
 *
 * Toggles internal magnetometer aiding in the device filter.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_INT_MAG_EN, 0);

/**
 * Enable MicroStrain external magnetometer aiding
 *
 * Toggles external magnetometer aiding in the device filter.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_EXT_MAG_EN, 0);

/**
 * Enable MicroStrain internal heading aiding
 *
 * Toggles internal heading as an aiding measurement.
 * If dual antennas are supported (CV7-GNSS/INS). The filter will be configured to use dual antenna heading as an aiding measurement.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_INT_HEAD_EN, 0);

/**
 * Enable MicroStrain external heading aiding
 *
 * Toggles external heading as an aiding measurement.
 * If enabled, the filter will be configured to accept external heading as an aiding meaurement.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_EXT_HEAD_EN, 0);

/**
 * Enable MicroStrain optical flow aiding
 *
 * Toggles body frame velocity as an aiding measurement.
 * The driver uses the body frame velocity from the optical flow sensor as the aiding measurements.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_OPT_FLOW_EN, 0);

/**
 * Enables Microstrain sensor to vehicle transform
 *
 * If the sensor has a different orientation with respect to the vehicle. This will enable a transform to correct itself.
 * The transform is described by MS_SENSOR_ROLL, MS_SENSOR_PITCH, MS_SENSOR_YAW.
 * 
 *
 * @group Sensors
 * @value 0 Disabled
 * @value 1 Enabled
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_SVT_EN, 0);

/**
 * MicroStrain accelerometer range
 *
 * -1 = Will not be configured, and will use the device default range.
 * Ranges vary by device and map to integer codes. Check the device's [User Manual](https://www.hbkworld.com/en/products/transducers/inertial-sensors#!ref_microstrain.com) for supported ranges and set the corresponding integer.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_ACCEL_RANGE, -1);

/**
 * MicroStrain gyroscope range
 *
 * -1 = Will not be configured, and will use the device default range.
 * Ranges vary by device and map to integer codes. Check the device's [User Manual](https://www.hbkworld.com/en/products/transducers/inertial-sensors#!ref_microstrain.com) for supported ranges and set the corresponding integer.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MS_GYRO_RANGE, -1);

/**
 * MicroStrain GNSS lever arm offset 1 (X)
 *
 * Lever arm offset (m) in the X direction for the external GNSS receiver.
 * In the case of a dual antenna setup, this is antenna 1.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF1_X, 0.0);

/**
 * MicroStrain GNSS lever arm offset 1 (Y)
 *
 * Lever arm offset (m) in the Y direction for the external GNSS receiver.
 * In the case of a dual antenna setup, this is antenna 1.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF1_Y, 0.0);

/**
 * MicroStrain GNSS lever arm offset 1 (Z)
 *
 * Lever arm offset (m) in the Z direction for the external GNSS receiver.
 * In the case of a dual antenna setup, this is antenna 1.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF1_Z, 0.0);

/**
 * MicroStrain GNSS lever arm offset 2 (X)
 *
 * Lever arm offset (m) in the X direction for antenna 2
 * This will only be used if the device supports a dual antenna setup.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF2_X, 0.0);

/**
 * MicroStrain GNSS lever arm offset 2 (Y)
 *
 * Lever arm offset (m) in the Y direction for antenna 2.
 * This will only be used if the device supports a dual antenna setup.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF2_Y, 0.0);

/**
 * MicroStrain GNSS lever arm offset 2 (Z)
 *
 * Lever arm offset (m) in the X direction for antenna 2.
 * This will only be used if the device supports a dual antenna setup.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_GNSS_OFF2_Z, 0.0);

/**
 * MicroStrain Sensor to vehicle transform (Roll)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the x axis.
 * Requires MS_SVT_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_SENSOR_ROLL, 0.0);

/**
 * MicroStrain Sensor to Vehicle Transform (Pitch)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the y axis.
 * Requires MS_SVT_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_SENSOR_PTCH, 0.0);

/**
 * MicroStrain Sensor to Vehicle Transform (Yaw)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the z axis.
 * Requires MS_SVT_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_SENSOR_YAW, 0.0);

/**
 * MicroStrain External Magnetometer Orientation (Roll)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the x axis.
 * Requires MS_EXT_MAG_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_EMAG_ROLL, 0.0);

/**
 * MicroStrain External Magnetometer Orientation (Pitch)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the y axis.
 * Requires MS_EXT_MAG_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_EMAG_PTCH, 0.0);

/**
 * MicroStrain External Magnetometer Orientation (Yaw)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the z axis.
 * Requires MS_EXT_MAG_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_EMAG_YAW, 0.0);

/**
 * MicroStrain optical flow offset (X)
 *
 * Offset (m) in the X direction if an Optical Flow sensor is connected.
 * Requires MS_OPT_FLOW_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_OFLW_OFF_X, 0.0);

/**
 * MicroStrain optical flow offset (Y)
 *
 * Offset (m) in the Y direction if an Optical Flow sensor is connected.
 * Requires MS_OPT_FLOW_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_OFLW_OFF_Y, 0.0);

/**
 * MicroStrain optical flow offset (Z)
 *
 * Offset (m) in the Z direction if an Optical Flow sensor is connected.
 * Requires MS_OPT_FLOW_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_OFLW_OFF_Z, 0.0);

/**
 * MicroStrain External Heading Orientation (Yaw)
 *
 * The orientation of the device (Radians) with respect to the vehicle frame around the z axis.
 * Requires MS_EXT_HEAD_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_EHEAD_YAW, 0.0);

/**
 * MicroStrain external magnetometer uncertainty
 *
 * The 1-sigma uncertainty (in Gauss) for all axes, which will remain constant across all aiding measurements.
 * Requires MS_EXT_MAG_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_EMAG_UNCERT, 0.1);

/**
 * MicroStrain optical flow uncertainty
 *
 * The 1-sigma uncertainty (in m/s) for the X and Y axes, which will remain constant across all aiding measurements.
 * The Z axis is not used for aiding.
 * Requires MS_OPT_FLOW_EN to be enabled to be used.
 * 
 *
 * @group Sensors
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MS_OFLW_UNCERT, 0.1);

/**
 * sbgECom driver mode
 *
 * Modes available for sbgECom driver.
 * In Sensors Only mode, use external IMU and magnetometer.
 * In GNSS mode, use external GNSS in addition to sensors only mode.
 * In INS mode, use external Kalman Filter in addition to GNSS mode.
 * 
 * In INS mode, requires EKF2_EN 0. Keeping both enabled
 * can lead to an unexpected behavior and vehicle instability.
 * 
 *
 * @group Sensors
 * @value 0 Sensors Only
 * @value 1 GNSS
 * @value 2 INS (default)
 * @category System
 */
PARAM_DEFINE_INT32(SBG_MODE, 2);

/**
 * sbgECom driver baudrate
 *
 * Baudrate used by default for serial communication between PX4
 * and SBG Systems INS through sbgECom driver.
 * 
 *
 * @group Sensors
 * @category System
 * @min 9600
 * @max 921600
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SBG_BAUDRATE, 921600);

/**
 * sbgECom driver INS configuration enable
 *
 * Enable SBG Systems INS configuration through sbgECom driver
 * on start.
 * 
 *
 * @group Sensors
 * @boolean
 * @category System
 */
PARAM_DEFINE_INT32(SBG_CONFIGURE_EN, 0);

/**
 * VectorNav driver mode
 *
 * INS or sensors
 *
 * @group Sensors
 * @value 0 Sensors Only (default)
 * @value 1 INS
 * @category System
 */
PARAM_DEFINE_INT32(VN_MODE, 0);

/**
 * BMM350 ODR rate
 *
 * Defines which ODR rate to use during data polling.
 * 
 *
 * @group Magnetometer
 * @value 0 400 Hz
 * @value 1 200 Hz
 * @value 2 100 Hz
 * @value 3 50 Hz
 * @value 4 25 Hz
 * @value 5 12.5 Hz
 * @value 6 6.25 Hz
 * @value 7 3.125 Hz
 * @value 8 1.5625 Hz
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BMM350_ODR, 3);

/**
 * BMM350 data averaging
 *
 * Defines which averaging mode to use during data polling.
 * 
 *
 * @group Magnetometer
 * @value 0 No averaging
 * @value 1 2 sample averaging
 * @value 2 4 sample averaging
 * @value 3 8 sample averaging
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BMM350_AVG, 1);

/**
 * BMM350 pad drive strength setting
 *
 * This setting helps avoid signal problems like overshoot or undershoot.
 * 
 *
 * @group Magnetometer
 * @min 0
 * @max 7
 */
PARAM_DEFINE_INT32(BMM350_DRIVE, 7);

/**
 * OSD Symbol Selection
 *
 * Configure / toggle support display options.
 * 
 *
 * @group OSD
 * @bit 0 CRAFT_NAME
 * @bit 1 DISARMED
 * @bit 2 GPS_LAT
 * @bit 3 GPS_LON
 * @bit 4 GPS_SATS
 * @bit 5 GPS_SPEED
 * @bit 6 HOME_DIST
 * @bit 7 HOME_DIR
 * @bit 8 MAIN_BATT_VOLTAGE
 * @bit 9 CURRENT_DRAW
 * @bit 10 MAH_DRAWN
 * @bit 11 RSSI_VALUE
 * @bit 12 ALTITUDE
 * @bit 13 NUMERICAL_VARIO
 * @bit 14 (unused) FLYMODE
 * @bit 15 (unused) ESC_TMP
 * @bit 16 (unused) PITCH_ANGLE
 * @bit 17 (unused) ROLL_ANGLE
 * @bit 18 CROSSHAIRS
 * @bit 19 AVG_CELL_VOLTAGE
 * @bit 20 (unused) HORIZON_SIDEBARS
 * @bit 21 POWER
 * @min 0
 * @max 4194303
 */
PARAM_DEFINE_INT32(OSD_SYMBOLS, 16383);

/**
 * OSD Crosshairs Height
 *
 * Controls the vertical position of the crosshair display.
 * Resolution is limited by OSD to 15 discrete values. Negative
 * values will display the crosshairs below the horizon
 * 
 *
 * @group OSD
 * @min -8
 * @max 8
 */
PARAM_DEFINE_INT32(OSD_CH_HEIGHT, 0);

/**
 * OSD Warning Level
 *
 * Minimum security of log level to display on the OSD.
 * 
 *
 * @group OSD
 */
PARAM_DEFINE_INT32(OSD_LOG_LEVEL, 3);

/**
 * OSD Scroll Rate (ms)
 *
 * Scroll rate in milliseconds for OSD messages longer than available character width.
 * This is lower-bounded by the nominal loop rate of this module.
 * 
 *
 * @group OSD
 * @min 100
 * @max 1000
 */
PARAM_DEFINE_INT32(OSD_SCROLL_RATE, 125);

/**
 * OSD Dwell Time (ms)
 *
 * Amount of time in milliseconds to dwell at the beginning of the display, when scrolling.
 * 
 *
 * @group OSD
 * @min 100
 * @max 10000
 */
PARAM_DEFINE_INT32(OSD_DWELL_TIME, 500);

/**
 * OSD RC Stick commands
 *
 * Forward RC stick input to VTX when disarmed
 * 
 *
 * @group OSD
 * @min 0
 * @max 1
 */
PARAM_DEFINE_INT32(OSD_RC_STICK, 1);

/**
 * Crossfire RC telemetry enable
 *
 * Crossfire telemetry enable
 * 
 *
 * @group RC
 * @boolean
 */
PARAM_DEFINE_INT32(RC_CRSF_TEL_EN, 0);

/**
 * Ghost RC telemetry enable
 *
 * Ghost telemetry enable
 * 
 *
 * @group RC
 * @boolean
 */
PARAM_DEFINE_INT32(RC_GHST_TEL_EN, 0);

/**
 * RC input protocol
 *
 * Select your RC input protocol or auto to scan.
 * 
 *
 * @group RC Input
 * @value -1 Auto
 * @value 0 None
 * @value 1 PPM
 * @value 2 SBUS
 * @value 3 DSM
 * @value 4 ST24
 * @value 5 SUMD
 * @value 6 CRSF
 * @value 7 GHST
 * @category System
 * @min -1
 * @max 7
 */
PARAM_DEFINE_INT32(RC_INPUT_PROTO, -1);

/**
 * Number of encoder counts for one wheel revolution
 *
 * The default value of 1200 corresponds to the default configuration of the Aion R1 rover.
 *
 * @group Roboclaw Driver
 * @min 1
 */
PARAM_DEFINE_INT32(RBCLW_COUNTS_REV, 1200);

/**
 * Address of the ESC on the bus
 *
 * The ESC has to be configured to have an address from 0x80 to 0x87. This parameter needs to match the configured value.
 *
 * @group Roboclaw Driver
 * @value 128 0x80
 * @value 129 0x81
 * @value 130 0x82
 * @value 131 0x83
 * @value 132 0x84
 * @value 133 0x85
 * @value 134 0x86
 * @value 135 0x87
 * @min 128
 * @max 135
 */
PARAM_DEFINE_INT32(RBCLW_ADDRESS, 128);

/**
 * Roboclaw Driver Channel 1 Output Function
 *
 * Select what should be output on Roboclaw Driver Channel 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(RBCLW_FUNC1, 0);

/**
 * Roboclaw Driver Channel 2 Output Function
 *
 * Select what should be output on Roboclaw Driver Channel 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(RBCLW_FUNC2, 0);

/**
 * Roboclaw Driver Channel 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 128
 * @max 128
 */
PARAM_DEFINE_INT32(RBCLW_DIS1, 128);

/**
 * Roboclaw Driver Channel 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 128
 * @max 128
 */
PARAM_DEFINE_INT32(RBCLW_DIS2, 128);

/**
 * Roboclaw Driver Channel 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 1
 * @max 128
 */
PARAM_DEFINE_INT32(RBCLW_MIN1, 1);

/**
 * Roboclaw Driver Channel 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 1
 * @max 128
 */
PARAM_DEFINE_INT32(RBCLW_MIN2, 1);

/**
 * Roboclaw Driver Channel 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 128
 * @max 256
 */
PARAM_DEFINE_INT32(RBCLW_MAX1, 256);

/**
 * Roboclaw Driver Channel 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 128
 * @max 256
 */
PARAM_DEFINE_INT32(RBCLW_MAX2, 256);

/**
 * Roboclaw Driver Channel 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see RBCLW_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 257
 */
PARAM_DEFINE_INT32(RBCLW_FAIL1, -1);

/**
 * Roboclaw Driver Channel 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see RBCLW_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 257
 */
PARAM_DEFINE_INT32(RBCLW_FAIL2, -1);

/**
 * Reverse Output Range for Roboclaw Driver
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 Roboclaw Driver Channel 1
 * @bit 1 Roboclaw Driver Channel 2
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(RBCLW_REV, 0);

/**
 * TAP ESC Output ESC 1 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC1, 0);

/**
 * TAP ESC Output ESC 2 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC2, 0);

/**
 * TAP ESC Output ESC 3 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC3, 0);

/**
 * TAP ESC Output ESC 4 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC4, 0);

/**
 * TAP ESC Output ESC 5 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC5, 0);

/**
 * TAP ESC Output ESC 6 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC6, 0);

/**
 * TAP ESC Output ESC 7 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC7, 0);

/**
 * TAP ESC Output ESC 8 Output Function
 *
 * Select what should be output on TAP ESC Output ESC 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(TAP_ESC_FUNC8, 0);

/**
 * Reverse Output Range for TAP ESC Output
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 TAP ESC Output ESC 1
 * @bit 1 TAP ESC Output ESC 2
 * @bit 2 TAP ESC Output ESC 3
 * @bit 3 TAP ESC Output ESC 4
 * @bit 4 TAP ESC Output ESC 5
 * @bit 5 TAP ESC Output ESC 6
 * @bit 6 TAP ESC Output ESC 7
 * @bit 7 TAP ESC Output ESC 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(TAP_ESC_REV, 0);

/**
 * Enable TMP102
 *
 * Enable the driver for the TMP102 temperature sensor
 * 
 *
 * @group Sensors
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SENS_EN_TMP102, 0);

/**
 * Which CAN interfaces to output ESC messages on.
 *
 * Since ESC messages are high priority and sent at a high rate, it is
 * recommended to only enable the interfaces that are actually used.
 * Otherwise, the ESC messages will arbitrate lower priority messages and
 * starve other nodes on the bus.
 * 
 *
 * @group UAVCAN
 * @bit 0 CAN1
 * @bit 1 CAN2
 * @bit 2 CAN3
 * @bit 3 CAN4
 * @bit 4 CAN5
 * @bit 5 CAN6
 * @bit 6 CAN7
 * @bit 7 CAN8
 * @min 0
 * @max 255
 * @min 1
 * @max 255
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UAVCAN_ESC_IFACE, 255);

/**
 * Number of UAVCAN lights to configure
 *
 * Number of lights to control via UAVCAN LightsCommand messages.
 * Set to 0 to disable UAVCAN light control.
 * Each light uses two parameters: LGT_IDx for the light_id and LGT_FNx for the function.
 * 
 *
 * @group UAVCAN
 * @min 0
 * @max 2
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UAVCAN_LGT_NUM, 1);

/**
 * Light 0 ID
 *
 * specifies the light_id value for light 0 in UAVCAN LightsCommand messages.
 * This determines which physical LED responds to commands for this light slot.
 * 
 *
 * @group UAVCAN
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(UAVCAN_LGT_ID0, 0);

/**
 * Light 1 ID
 *
 * specifies the light_id value for light 1 in UAVCAN LightsCommand messages.
 * This determines which physical LED responds to commands for this light slot.
 * 
 *
 * @group UAVCAN
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(UAVCAN_LGT_ID1, 0);

/**
 * Light 0 function
 *
 * Function for light 0.
 * UAVCAN_LGT_MODE determines when the first option or second option is active Off/On.
 * 
 *
 * @group UAVCAN
 * @value 0 Status/Status
 * @value 1 Off/White
 * @value 2 Off/Red
 * @value 3 Off/Green
 * @value 4 Status/White
 * @value 5 Status/Red
 * @value 6 Status/Green
 * @value 7 Status/Off
 * @min 0
 * @max 9
 */
PARAM_DEFINE_INT32(UAVCAN_LGT_FN0, 0);

/**
 * Light 1 function
 *
 * Function for light 1.
 * UAVCAN_LGT_MODE determines when the first option or second option is active Off/On.
 * 
 *
 * @group UAVCAN
 * @value 0 Status/Status
 * @value 1 Off/White
 * @value 2 Off/Red
 * @value 3 Off/Green
 * @value 4 Status/White
 * @value 5 Status/Red
 * @value 6 Status/Green
 * @value 7 Status/Off
 * @min 0
 * @max 9
 */
PARAM_DEFINE_INT32(UAVCAN_LGT_FN1, 0);

/**
 * UAVCAN ESC 1 Output Function
 *
 * Select what should be output on UAVCAN ESC 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC1, 0);

/**
 * UAVCAN ESC 2 Output Function
 *
 * Select what should be output on UAVCAN ESC 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC2, 0);

/**
 * UAVCAN ESC 3 Output Function
 *
 * Select what should be output on UAVCAN ESC 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC3, 0);

/**
 * UAVCAN ESC 4 Output Function
 *
 * Select what should be output on UAVCAN ESC 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC4, 0);

/**
 * UAVCAN ESC 5 Output Function
 *
 * Select what should be output on UAVCAN ESC 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC5, 0);

/**
 * UAVCAN ESC 6 Output Function
 *
 * Select what should be output on UAVCAN ESC 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC6, 0);

/**
 * UAVCAN ESC 7 Output Function
 *
 * Select what should be output on UAVCAN ESC 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC7, 0);

/**
 * UAVCAN ESC 8 Output Function
 *
 * Select what should be output on UAVCAN ESC 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FUNC8, 0);

/**
 * UAVCAN ESC 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN1, 1);

/**
 * UAVCAN ESC 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN2, 1);

/**
 * UAVCAN ESC 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN3, 1);

/**
 * UAVCAN ESC 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN4, 1);

/**
 * UAVCAN ESC 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN5, 1);

/**
 * UAVCAN ESC 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN6, 1);

/**
 * UAVCAN ESC 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN7, 1);

/**
 * UAVCAN ESC 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MIN8, 1);

/**
 * UAVCAN ESC 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX1, 8191);

/**
 * UAVCAN ESC 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX2, 8191);

/**
 * UAVCAN ESC 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX3, 8191);

/**
 * UAVCAN ESC 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX4, 8191);

/**
 * UAVCAN ESC 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX5, 8191);

/**
 * UAVCAN ESC 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX6, 8191);

/**
 * UAVCAN ESC 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX7, 8191);

/**
 * UAVCAN ESC 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_MAX8, 8191);

/**
 * UAVCAN ESC 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL1, -1);

/**
 * UAVCAN ESC 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL2, -1);

/**
 * UAVCAN ESC 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL3, -1);

/**
 * UAVCAN ESC 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL4, -1);

/**
 * UAVCAN ESC 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL5, -1);

/**
 * UAVCAN ESC 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL6, -1);

/**
 * UAVCAN ESC 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL7, -1);

/**
 * UAVCAN ESC 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_EC_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 8191
 */
PARAM_DEFINE_INT32(UAVCAN_EC_FAIL8, -1);

/**
 * UAVCAN Servo 1 Output Function
 *
 * Select what should be output on UAVCAN Servo 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC1, 0);

/**
 * UAVCAN Servo 2 Output Function
 *
 * Select what should be output on UAVCAN Servo 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC2, 0);

/**
 * UAVCAN Servo 3 Output Function
 *
 * Select what should be output on UAVCAN Servo 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC3, 0);

/**
 * UAVCAN Servo 4 Output Function
 *
 * Select what should be output on UAVCAN Servo 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC4, 0);

/**
 * UAVCAN Servo 5 Output Function
 *
 * Select what should be output on UAVCAN Servo 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC5, 0);

/**
 * UAVCAN Servo 6 Output Function
 *
 * Select what should be output on UAVCAN Servo 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC6, 0);

/**
 * UAVCAN Servo 7 Output Function
 *
 * Select what should be output on UAVCAN Servo 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC7, 0);

/**
 * UAVCAN Servo 8 Output Function
 *
 * Select what should be output on UAVCAN Servo 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FUNC8, 0);

/**
 * UAVCAN Servo 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS1, 500);

/**
 * UAVCAN Servo 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS2, 500);

/**
 * UAVCAN Servo 3 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS3, 500);

/**
 * UAVCAN Servo 4 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS4, 500);

/**
 * UAVCAN Servo 5 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS5, 500);

/**
 * UAVCAN Servo 6 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS6, 500);

/**
 * UAVCAN Servo 7 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS7, 500);

/**
 * UAVCAN Servo 8 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_DIS8, 500);

/**
 * UAVCAN Servo 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN1, 0);

/**
 * UAVCAN Servo 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN2, 0);

/**
 * UAVCAN Servo 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN3, 0);

/**
 * UAVCAN Servo 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN4, 0);

/**
 * UAVCAN Servo 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN5, 0);

/**
 * UAVCAN Servo 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN6, 0);

/**
 * UAVCAN Servo 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN7, 0);

/**
 * UAVCAN Servo 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MIN8, 0);

/**
 * UAVCAN Servo 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX1, 1000);

/**
 * UAVCAN Servo 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX2, 1000);

/**
 * UAVCAN Servo 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX3, 1000);

/**
 * UAVCAN Servo 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX4, 1000);

/**
 * UAVCAN Servo 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX5, 1000);

/**
 * UAVCAN Servo 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX6, 1000);

/**
 * UAVCAN Servo 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX7, 1000);

/**
 * UAVCAN Servo 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_MAX8, 1000);

/**
 * UAVCAN Servo 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL1, -1);

/**
 * UAVCAN Servo 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL2, -1);

/**
 * UAVCAN Servo 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL3, -1);

/**
 * UAVCAN Servo 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL4, -1);

/**
 * UAVCAN Servo 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL5, -1);

/**
 * UAVCAN Servo 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL6, -1);

/**
 * UAVCAN Servo 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL7, -1);

/**
 * UAVCAN Servo 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see UAVCAN_SV_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(UAVCAN_SV_FAIL8, -1);

/**
 * Reverse Output Range for UAVCAN
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 UAVCAN ESC 1
 * @bit 1 UAVCAN ESC 2
 * @bit 2 UAVCAN ESC 3
 * @bit 3 UAVCAN ESC 4
 * @bit 4 UAVCAN ESC 5
 * @bit 5 UAVCAN ESC 6
 * @bit 6 UAVCAN ESC 7
 * @bit 7 UAVCAN ESC 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(UAVCAN_EC_REV, 0);

/**
 * Reverse Output Range for UAVCAN
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 UAVCAN Servo 1
 * @bit 1 UAVCAN Servo 2
 * @bit 2 UAVCAN Servo 3
 * @bit 3 UAVCAN Servo 4
 * @bit 4 UAVCAN Servo 5
 * @bit 5 UAVCAN Servo 6
 * @bit 6 UAVCAN Servo 7
 * @bit 7 UAVCAN Servo 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(UAVCAN_SV_REV, 0);

/**
 * UWB sensor X offset in body frame
 *
 * UWB sensor positioning in relation to Drone in NED. X offset. A Positive offset results in a Position o
 *
 * @group UWB
 * @decimal 2
 * @increment 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(UWB_INIT_OFF_X, 0.0);

/**
 * UWB sensor Y offset in body frame
 *
 * UWB sensor positioning in relation to Drone in NED. Y offset.
 *
 * @group UWB
 * @decimal 2
 * @increment 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(UWB_INIT_OFF_Y, 0.0);

/**
 * UWB sensor Z offset in body frame
 *
 * UWB sensor positioning in relation to Drone in NED. Z offset.
 *
 * @group UWB
 * @decimal 2
 * @increment 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(UWB_INIT_OFF_Z, 0.0);

/**
 * UWB sensor orientation
 *
 * The orientation of the sensor relative to the forward direction of the body frame. Look up table in src/lib/conversion/rotation.h Default position is the antannaes downward facing, UWB board parallel with body frame.
 *
 * @group UWB
 * @value 0 ROTATION_NONE
 * @value 1 ROTATION_YAW_45
 * @value 2 ROTATION_YAW_90
 * @value 3 ROTATION_YAW_135
 * @value 4 ROTATION_YAW_180
 * @value 5 ROTATION_YAW_225
 * @value 6 ROTATION_YAW_270
 * @value 7 ROTATION_YAW_315
 * @value 8 ROTATION_ROLL_180
 * @value 9 ROTATION_ROLL_180_YAW_45
 * @value 10 ROTATION_ROLL_180_YAW_90
 * @value 11 ROTATION_ROLL_180_YAW_135
 * @value 12 ROTATION_PITCH_180
 * @value 13 ROTATION_ROLL_180_YAW_225
 * @value 14 ROTATION_ROLL_180_YAW_270
 * @value 15 ROTATION_ROLL_180_YAW_315
 * @value 16 ROTATION_ROLL_90
 * @value 17 ROTATION_ROLL_90_YAW_45
 * @value 18 ROTATION_ROLL_90_YAW_90
 * @value 19 ROTATION_ROLL_90_YAW_135
 * @value 20 ROTATION_ROLL_270
 * @value 21 ROTATION_ROLL_270_YAW_45
 * @value 22 ROTATION_ROLL_270_YAW_90
 * @value 23 ROTATION_ROLL_270_YAW_135
 * @value 24 ROTATION_PITCH_90
 * @value 25 ROTATION_PITCH_270
 * @value 26 ROTATION_PITCH_180_YAW_90
 * @value 27 ROTATION_PITCH_180_YAW_270
 * @value 28 ROTATION_ROLL_90_PITCH_90
 * @value 29 ROTATION_ROLL_180_PITCH_90
 * @value 30 ROTATION_ROLL_270_PITCH_90
 * @value 31 ROTATION_ROLL_90_PITCH_180
 * @value 32 ROTATION_ROLL_270_PITCH_180
 * @value 33 ROTATION_ROLL_90_PITCH_270
 * @value 34 ROTATION_ROLL_180_PITCH_270
 * @value 35 ROTATION_ROLL_270_PITCH_270
 * @value 36 ROTATION_ROLL_90_PITCH_180_YAW_90
 * @value 37 ROTATION_ROLL_90_YAW_270
 * @value 38 ROTATION_ROLL_90_PITCH_68_YAW_293
 * @value 39 ROTATION_PITCH_315
 * @value 40 ROTATION_ROLL_90_PITCH_315
 */
PARAM_DEFINE_INT32(UWB_SENS_ROT, 0);

/**
 * VOXL2 IO Output PWM Channel 1 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC1, 0);

/**
 * VOXL2 IO Output PWM Channel 2 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC2, 0);

/**
 * VOXL2 IO Output PWM Channel 3 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC3, 0);

/**
 * VOXL2 IO Output PWM Channel 4 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC4, 0);

/**
 * VOXL2 IO Output PWM Channel 5 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC5, 0);

/**
 * VOXL2 IO Output PWM Channel 6 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC6, 0);

/**
 * VOXL2 IO Output PWM Channel 7 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC7, 0);

/**
 * VOXL2 IO Output PWM Channel 8 Output Function
 *
 * Select what should be output on VOXL2 IO Output PWM Channel 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(VOXL2_IO_FUNC8, 0);

/**
 * Reverse Output Range for VOXL2 IO Output
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 VOXL2 IO Output PWM Channel 1
 * @bit 1 VOXL2 IO Output PWM Channel 2
 * @bit 2 VOXL2 IO Output PWM Channel 3
 * @bit 3 VOXL2 IO Output PWM Channel 4
 * @bit 4 VOXL2 IO Output PWM Channel 5
 * @bit 5 VOXL2 IO Output PWM Channel 6
 * @bit 6 VOXL2 IO Output PWM Channel 7
 * @bit 7 VOXL2 IO Output PWM Channel 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(VOXL2_IO_REV, 0);

/**
 * VTX band
 *
 * VTX table band 1-24
 *
 * @group VTX
 * @value 0 Band 1
 * @value 1 Band 2
 * @value 2 Band 3
 * @value 3 Band 4
 * @value 4 Band 5
 * @value 5 Band 6
 * @value 6 Band 7
 * @value 7 Band 8
 * @value 8 Band 9
 * @value 9 Band 10
 * @value 10 Band 11
 * @value 11 Band 12
 * @value 12 Band 13
 * @value 13 Band 14
 * @value 14 Band 15
 * @value 15 Band 16
 * @value 16 Band 17
 * @value 17 Band 18
 * @value 18 Band 19
 * @value 19 Band 20
 * @value 20 Band 21
 * @value 21 Band 22
 * @value 22 Band 23
 * @value 23 Band 24
 */
PARAM_DEFINE_INT32(VTX_BAND, 0);

/**
 * VTX channel
 *
 * VTX table channel 1-16
 *
 * @group VTX
 * @value 0 Channel 1
 * @value 1 Channel 2
 * @value 2 Channel 3
 * @value 3 Channel 4
 * @value 4 Channel 5
 * @value 5 Channel 6
 * @value 6 Channel 7
 * @value 7 Channel 8
 * @value 8 Channel 9
 * @value 9 Channel 10
 * @value 10 Channel 11
 * @value 11 Channel 12
 * @value 12 Channel 13
 * @value 13 Channel 14
 * @value 14 Channel 15
 * @value 15 Channel 16
 */
PARAM_DEFINE_INT32(VTX_CHANNEL, 0);

/**
 * VTX frequency in MHz
 *
 * If the VTX frequency is set, it will overwrite the band and channel
 * settings. Set to 0 to use band and channel settings.
 * 
 *
 * @group VTX
 * @min 0
 * @max 32000
 */
PARAM_DEFINE_INT32(VTX_FREQUENCY, 0);

/**
 * VTX power level
 *
 * VTX transmission power level 1-16
 *
 * @group VTX
 * @value 0 Level 1
 * @value 1 Level 2
 * @value 2 Level 3
 * @value 3 Level 4
 * @value 4 Level 5
 * @value 5 Level 6
 * @value 6 Level 7
 * @value 7 Level 8
 * @value 8 Level 9
 * @value 9 Level 10
 * @value 10 Level 11
 * @value 11 Level 12
 * @value 12 Level 13
 * @value 13 Level 14
 * @value 14 Level 15
 * @value 15 Level 16
 */
PARAM_DEFINE_INT32(VTX_POWER, 0);

/**
 * VTX pit mode
 *
 * VTX pit mode reduces power to the minimum
 *
 * @group VTX
 * @boolean
 */
PARAM_DEFINE_INT32(VTX_PIT_MODE, 0);

/**
 * VTX mapping configuration
 *
 * Configure how VTX settings are controlled. Options include using
 * MSP commands, AUX channels or a combination of both. To use MSP
 * commands, you must use a CRSF receiver with MSP support enabled in
 * the PX4 build.
 * 
 *
 * @group VTX
 * @value 0 Disabled
 * @value 1 MSP for Power, Band and Channel
 * @value 2 MSP for Band and Channel, AUX for Power
 * @value 3 MSP for Power, AUX for Band and Channel
 * @value 4 AUX for Power, Band and Channel
 */
PARAM_DEFINE_INT32(VTX_MAP_CONFIG, 0);

/**
 * VTX device
 *
 * Specific VTX device useful for workarounds and optimizations
 *
 * @group VTX
 * @value 0 SmartAudio v1, v2, v2.1 Protocol
 * @value 100 Tramp Protocol
 * @value 5120 Peak THOR T67
 * @value 10240 Rush MAX SOLO
 * @reboot_required True
 */
PARAM_DEFINE_INT32(VTX_DEVICE, 0);

/**
 * Empty cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered empty.
 * The voltage should be chosen above the steep dropoff at 3.5V. A typical
 * lithium battery can only be discharged under high load down to 10% before
 * it drops off to a voltage level damaging the cells.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_V_EMPTY, 3.6);

/**
 * Empty cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered empty.
 * The voltage should be chosen above the steep dropoff at 3.5V. A typical
 * lithium battery can only be discharged under high load down to 10% before
 * it drops off to a voltage level damaging the cells.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_V_EMPTY, 3.6);

/**
 * Empty cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered empty.
 * The voltage should be chosen above the steep dropoff at 3.5V. A typical
 * lithium battery can only be discharged under high load down to 10% before
 * it drops off to a voltage level damaging the cells.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT3_V_EMPTY, 3.6);

/**
 * Full cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered full.
 * For a more accurate estimate set this below the nominal voltage of e.g. 4.2V
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_V_CHARGED, 4.05);

/**
 * Full cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered full.
 * For a more accurate estimate set this below the nominal voltage of e.g. 4.2V
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_V_CHARGED, 4.05);

/**
 * Full cell voltage
 *
 * Defines the voltage where a single cell of the battery is considered full.
 * For a more accurate estimate set this below the nominal voltage of e.g. 4.2V
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @unit V
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT3_V_CHARGED, 4.05);

/**
 * Explicitly defines the per cell internal resistance for battery 1
 *
 * If non-negative, then this will be used instead of the online estimated internal resistance.
 * 
 *
 * @group Battery Calibration
 * @decimal 4
 * @increment 0.0005
 * @min -1.0
 * @max 0.2
 * @unit Ohm
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_R_INTERNAL, -1.0);

/**
 * Explicitly defines the per cell internal resistance for battery 2
 *
 * If non-negative, then this will be used instead of the online estimated internal resistance.
 * 
 *
 * @group Battery Calibration
 * @decimal 4
 * @increment 0.0005
 * @min -1.0
 * @max 0.2
 * @unit Ohm
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_R_INTERNAL, -1.0);

/**
 * Explicitly defines the per cell internal resistance for battery 3
 *
 * If non-negative, then this will be used instead of the online estimated internal resistance.
 * 
 *
 * @group Battery Calibration
 * @decimal 4
 * @increment 0.0005
 * @min -1.0
 * @max 0.2
 * @unit Ohm
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT3_R_INTERNAL, -1.0);

/**
 * Number of cells for battery 1.
 *
 * Defines the number of cells the attached battery consists of.
 * 
 *
 * @group Battery Calibration
 * @value 0 Unknown
 * @value 1 1S Battery
 * @value 2 2S Battery
 * @value 3 3S Battery
 * @value 4 4S Battery
 * @value 5 5S Battery
 * @value 6 6S Battery
 * @value 7 7S Battery
 * @value 8 8S Battery
 * @value 9 9S Battery
 * @value 10 10S Battery
 * @value 11 11S Battery
 * @value 12 12S Battery
 * @value 13 13S Battery
 * @value 14 14S Battery
 * @value 15 15S Battery
 * @value 16 16S Battery
 */
PARAM_DEFINE_INT32(BAT1_N_CELLS, 0);

/**
 * Number of cells for battery 2.
 *
 * Defines the number of cells the attached battery consists of.
 * 
 *
 * @group Battery Calibration
 * @value 0 Unknown
 * @value 1 1S Battery
 * @value 2 2S Battery
 * @value 3 3S Battery
 * @value 4 4S Battery
 * @value 5 5S Battery
 * @value 6 6S Battery
 * @value 7 7S Battery
 * @value 8 8S Battery
 * @value 9 9S Battery
 * @value 10 10S Battery
 * @value 11 11S Battery
 * @value 12 12S Battery
 * @value 13 13S Battery
 * @value 14 14S Battery
 * @value 15 15S Battery
 * @value 16 16S Battery
 */
PARAM_DEFINE_INT32(BAT2_N_CELLS, 0);

/**
 * Number of cells for battery 3.
 *
 * Defines the number of cells the attached battery consists of.
 * 
 *
 * @group Battery Calibration
 * @value 0 Unknown
 * @value 1 1S Battery
 * @value 2 2S Battery
 * @value 3 3S Battery
 * @value 4 4S Battery
 * @value 5 5S Battery
 * @value 6 6S Battery
 * @value 7 7S Battery
 * @value 8 8S Battery
 * @value 9 9S Battery
 * @value 10 10S Battery
 * @value 11 11S Battery
 * @value 12 12S Battery
 * @value 13 13S Battery
 * @value 14 14S Battery
 * @value 15 15S Battery
 * @value 16 16S Battery
 */
PARAM_DEFINE_INT32(BAT3_N_CELLS, 0);

/**
 * Battery 1 capacity.
 *
 * Defines the capacity of battery 1 in mAh.
 * 
 *
 * @group Battery Calibration
 * @decimal 0
 * @increment 50
 * @min -1.0
 * @max 100000
 * @unit mAh
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_CAPACITY, -1.0);

/**
 * Battery 2 capacity.
 *
 * Defines the capacity of battery 2 in mAh.
 * 
 *
 * @group Battery Calibration
 * @decimal 0
 * @increment 50
 * @min -1.0
 * @max 100000
 * @unit mAh
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_CAPACITY, -1.0);

/**
 * Battery 3 capacity.
 *
 * Defines the capacity of battery 3 in mAh.
 * 
 *
 * @group Battery Calibration
 * @decimal 0
 * @increment 50
 * @min -1.0
 * @max 100000
 * @unit mAh
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT3_CAPACITY, -1.0);

/**
 * Battery 1 monitoring source.
 *
 * This parameter controls the source of battery data. The value 'Power Module / Analog'
 * means that measurements are expected to come from either analog (ADC) inputs
 * or an I2C power monitor (e.g. INA226). Analog inputs are voltage and current
 * measurements read from the board's ADC channels, typically from an onboard
 * voltage divider and current shunt, or an external analog power module.
 * I2C power monitors are digital sensors on the I2C bus.
 * If the value is set to 'External' then the system expects to receive MAVLink
 * or CAN battery status messages, or the battery data is published by an external driver.
 * If the value is set to 'ESCs', the battery information are taken from the esc_status message.
 * This requires the ESC to provide both voltage as well as current (via ESC telemetry).
 * 
 *
 * @group Battery Calibration
 * @value -1 Disabled
 * @value 0 Power Module / Analog
 * @value 1 External
 * @value 2 ESCs
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BAT1_SOURCE, 0);

/**
 * Battery 2 monitoring source.
 *
 * This parameter controls the source of battery data. The value 'Power Module / Analog'
 * means that measurements are expected to come from either analog (ADC) inputs
 * or an I2C power monitor (e.g. INA226). Analog inputs are voltage and current
 * measurements read from the board's ADC channels, typically from an onboard
 * voltage divider and current shunt, or an external analog power module.
 * I2C power monitors are digital sensors on the I2C bus.
 * If the value is set to 'External' then the system expects to receive MAVLink
 * or CAN battery status messages, or the battery data is published by an external driver.
 * If the value is set to 'ESCs', the battery information are taken from the esc_status message.
 * This requires the ESC to provide both voltage as well as current (via ESC telemetry).
 * 
 *
 * @group Battery Calibration
 * @value -1 Disabled
 * @value 0 Power Module / Analog
 * @value 1 External
 * @value 2 ESCs
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BAT2_SOURCE, -1);

/**
 * Battery 3 monitoring source.
 *
 * This parameter controls the source of battery data. The value 'Power Module / Analog'
 * means that measurements are expected to come from either analog (ADC) inputs
 * or an I2C power monitor (e.g. INA226). Analog inputs are voltage and current
 * measurements read from the board's ADC channels, typically from an onboard
 * voltage divider and current shunt, or an external analog power module.
 * I2C power monitors are digital sensors on the I2C bus.
 * If the value is set to 'External' then the system expects to receive MAVLink
 * or CAN battery status messages, or the battery data is published by an external driver.
 * If the value is set to 'ESCs', the battery information are taken from the esc_status message.
 * This requires the ESC to provide both voltage as well as current (via ESC telemetry).
 * 
 *
 * @group Battery Calibration
 * @value -1 Disabled
 * @value 0 Power Module / Analog
 * @value 1 External
 * @value 2 ESCs
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BAT3_SOURCE, -1);

/**
 * Low threshold.
 *
 * Sets the threshold when the battery will be reported as low.
 * This has to be higher than the critical threshold.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @min 0.12
 * @max 0.5
 * @unit norm
 */
PARAM_DEFINE_FLOAT(BAT_LOW_THR, 0.15);

/**
 * Critical threshold.
 *
 * Sets the threshold when the battery will be reported as critically low.
 * This has to be lower than the low threshold. This threshold commonly
 * will trigger RTL.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @min 0.05
 * @max 0.5
 * @unit norm
 */
PARAM_DEFINE_FLOAT(BAT_CRIT_THR, 0.07);

/**
 * Emergency threshold.
 *
 * Sets the threshold when the battery will be reported as dangerously low.
 * This has to be lower than the critical threshold. This threshold commonly
 * will trigger landing.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.01
 * @min 0.03
 * @max 0.5
 * @unit norm
 */
PARAM_DEFINE_FLOAT(BAT_EMERGEN_THR, 0.05);

/**
 * Expected battery current in flight.
 *
 * This value is used to initialize the in-flight average current estimation,
 * which in turn is used for estimating remaining flight time and RTL triggering.
 * 
 *
 * @group Battery Calibration
 * @decimal 2
 * @increment 0.1
 * @min 0
 * @max 500
 * @unit A
 */
PARAM_DEFINE_FLOAT(BAT_AVRG_CURRENT, 15);

/**
 * Battery 1 voltage divider (V divider)
 *
 * This is the divider from battery 1 voltage to ADC voltage.
 * If using e.g. Mauch power modules the value from the datasheet
 * can be applied straight here. A value of -1 means to use
 * the board default.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_V_DIV, -1.0);

/**
 * Battery 2 voltage divider (V divider)
 *
 * This is the divider from battery 2 voltage to ADC voltage.
 * If using e.g. Mauch power modules the value from the datasheet
 * can be applied straight here. A value of -1 means to use
 * the board default.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_V_DIV, -1.0);

/**
 * Battery 1 current per volt (A/V)
 *
 * The voltage seen by the ADC multiplied by this factor
 * will determine the battery current. A value of -1 means to use
 * the board default.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_A_PER_V, -1.0);

/**
 * Battery 2 current per volt (A/V)
 *
 * The voltage seen by the ADC multiplied by this factor
 * will determine the battery current. A value of -1 means to use
 * the board default.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_A_PER_V, -1.0);

/**
 * Battery 1 Voltage ADC Channel
 *
 * This parameter specifies the ADC channel used to monitor voltage of main power battery.
 * A value of -1 means to use the board default. A value of -2 disables analog monitoring.
 * This is useful when the FMU supports both analog and digital voltage monitoring and you want
 * to use digital monitoring exclusively.
 * 
 *
 * @group Battery Calibration
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BAT1_V_CHANNEL, -1);

/**
 * Battery 2 Voltage ADC Channel
 *
 * This parameter specifies the ADC channel used to monitor voltage of main power battery.
 * A value of -1 means to use the board default. A value of -2 disables analog monitoring.
 * This is useful when the FMU supports both analog and digital voltage monitoring and you want
 * to use digital monitoring exclusively.
 * 
 *
 * @group Battery Calibration
 * @reboot_required True
 */
PARAM_DEFINE_INT32(BAT2_V_CHANNEL, -1);

/**
 * Battery 1 idle current overwrite
 *
 * This parameter allows to overwrite the current measured during
 * idle (unarmed) state with a user-defined constant value (expressed in amperes).
 * When the system is armed, the measured current is used. This is useful
 * because on certain ESCs current measurements are inaccurate in case of no load.
 * Negative values are ignored and will cause the measured current to be used.
 * The default value of 0 disables the overwrite, in which case the measured value
 * is always used.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT1_I_OVERWRITE, 0);

/**
 * Battery 2 idle current overwrite
 *
 * This parameter allows to overwrite the current measured during
 * idle (unarmed) state with a user-defined constant value (expressed in amperes).
 * When the system is armed, the measured current is used. This is useful
 * because on certain ESCs current measurements are inaccurate in case of no load.
 * Negative values are ignored and will cause the measured current to be used.
 * The default value of 0 disables the overwrite, in which case the measured value
 * is always used.
 * 
 *
 * @group Battery Calibration
 * @decimal 8
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(BAT2_I_OVERWRITE, 0);

/**
 * Battery 1 voltage filter time constant
 *
 * Low-pass filter time constant for the battery voltage ADC reading (in seconds).
 * A higher value results in more smoothing and less noise, but slower response.
 * A value of 0 disables the filter.
 * 
 *
 * @group Battery Calibration
 * @decimal 3
 * @min 0.0
 * @max 5.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(BAT1_V_FILT, 0.0);

/**
 * Battery 2 voltage filter time constant
 *
 * Low-pass filter time constant for the battery voltage ADC reading (in seconds).
 * A higher value results in more smoothing and less noise, but slower response.
 * A value of 0 disables the filter.
 * 
 *
 * @group Battery Calibration
 * @decimal 3
 * @min 0.0
 * @max 5.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(BAT2_V_FILT, 0.0);

/**
 * Battery 1 current filter time constant
 *
 * Low-pass filter time constant for the battery current ADC reading (in seconds).
 * A higher value results in more smoothing and less noise, but slower response.
 * A value of 0 disables the filter.
 * 
 *
 * @group Battery Calibration
 * @decimal 3
 * @min 0.0
 * @max 5.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(BAT1_I_FILT, 0.0);

/**
 * Battery 2 current filter time constant
 *
 * Low-pass filter time constant for the battery current ADC reading (in seconds).
 * A higher value results in more smoothing and less noise, but slower response.
 * A value of 0 disables the filter.
 * 
 *
 * @group Battery Calibration
 * @decimal 3
 * @min 0.0
 * @max 5.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(BAT2_I_FILT, 0.0);

/**
 * External mode identifier 0
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE0_HASH, 0);

/**
 * External mode identifier 1
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE1_HASH, 0);

/**
 * External mode identifier 2
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE2_HASH, 0);

/**
 * External mode identifier 3
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE3_HASH, 0);

/**
 * External mode identifier 4
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE4_HASH, 0);

/**
 * External mode identifier 5
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE5_HASH, 0);

/**
 * External mode identifier 6
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE6_HASH, 0);

/**
 * External mode identifier 7
 *
 * This parameter is automatically set to identify external modes. It ensures that modes
 * get assigned to the same index independent from their startup order,
 * which is required when mapping an external mode to an RC switch.
 * 
 *
 * @group Commander
 * @category System
 * @volatile True
 */
PARAM_DEFINE_INT32(COM_MODE7_HASH, 0);

/**
 * Mode slot 1
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE1, -1);

/**
 * Mode slot 2
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE2, -1);

/**
 * Mode slot 3
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE3, -1);

/**
 * Mode slot 4
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE4, -1);

/**
 * Mode slot 5
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE5, -1);

/**
 * Mode slot 6
 *
 * If the main switch channel is in this range the
 * selected flight mode will be applied.
 * 
 *
 * @group Commander
 * @value -1 Unassigned
 * @value 0 Manual
 * @value 1 Altitude
 * @value 2 Position
 * @value 9 Position Slow
 * @value 3 Mission
 * @value 4 Hold
 * @value 10 Takeoff
 * @value 11 Land
 * @value 5 Return
 * @value 6 Acro
 * @value 7 Offboard
 * @value 8 Stabilized
 * @value 12 Follow Me
 * @value 13 Precision Land
 * @value 16 Altitude Cruise
 * @value 100 External Mode 1
 * @value 101 External Mode 2
 * @value 102 External Mode 3
 * @value 103 External Mode 4
 * @value 104 External Mode 5
 * @value 105 External Mode 6
 * @value 106 External Mode 7
 * @value 107 External Mode 8
 */
PARAM_DEFINE_INT32(COM_FLTMODE6, -1);

/**
 * Airframe selection
 *
 * Defines which mixer implementation to use.
 * Some are generic, while others are specifically fit to a certain vehicle with a fixed set of actuators.
 * 
 * 'Custom' should only be used if noting else can be used.
 * 
 *
 * @group Geometry
 * @value 0 Multirotor
 * @value 1 Fixed-wing
 * @value 2 Standard VTOL
 * @value 3 Tiltrotor VTOL
 * @value 4 Tailsitter VTOL
 * @value 5 Rover (Ackermann)
 * @value 6 Rover (Differential)
 * @value 7 Motors (6DOF)
 * @value 8 Multirotor with Tilt
 * @value 9 Custom
 * @value 10 Helicopter (tail ESC)
 * @value 11 Helicopter (tail Servo)
 * @value 12 Helicopter (Coaxial)
 * @value 13 Rover (Mecanum)
 * @value 14 Spacecraft 2D
 * @value 15 Spacecraft 3D
 */
PARAM_DEFINE_INT32(CA_AIRFRAME, 0);

/**
 * Control allocation method
 *
 * Selects the algorithm and desaturation method.
 * If set to Automatic, the selection is based on the airframe (CA_AIRFRAME).
 * 
 *
 * @group Geometry
 * @value 0 Pseudo-inverse with output clipping
 * @value 1 Pseudo-inverse with sequential desaturation technique
 * @value 2 Automatic
 */
PARAM_DEFINE_INT32(CA_METHOD, 2);

/**
 * Bidirectional/Reversible motors
 *
 * Configure motors to be bidirectional/reversible. Note that the output driver needs to support this as well.
 * 
 *
 * @group Geometry
 * @bit 0 Motor 1
 * @bit 1 Motor 2
 * @bit 2 Motor 3
 * @bit 3 Motor 4
 * @bit 4 Motor 5
 * @bit 5 Motor 6
 * @bit 6 Motor 7
 * @bit 7 Motor 8
 * @bit 8 Motor 9
 * @bit 9 Motor 10
 * @bit 10 Motor 11
 * @bit 11 Motor 12
 * @min 0
 * @max 4095
 */
PARAM_DEFINE_INT32(CA_R_REV, 0);

/**
 * Motor 0 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R0_SLEW, 0.0);

/**
 * Motor 1 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R1_SLEW, 0.0);

/**
 * Motor 2 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R2_SLEW, 0.0);

/**
 * Motor 3 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R3_SLEW, 0.0);

/**
 * Motor 4 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R4_SLEW, 0.0);

/**
 * Motor 5 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R5_SLEW, 0.0);

/**
 * Motor 6 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R6_SLEW, 0.0);

/**
 * Motor 7 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R7_SLEW, 0.0);

/**
 * Motor 8 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R8_SLEW, 0.0);

/**
 * Motor 9 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R9_SLEW, 0.0);

/**
 * Motor 10 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R10_SLEW, 0.0);

/**
 * Motor 11 slew rate limit
 *
 * Forces the motor output signal to take at least the configured time (in seconds)
 * to traverse its full range (normally [0, 1], or if reversible [-1, 1]).
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_R11_SLEW, 0.0);

/**
 * Servo 0 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV0_SLEW, 0.0);

/**
 * Servo 1 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV1_SLEW, 0.0);

/**
 * Servo 2 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV2_SLEW, 0.0);

/**
 * Servo 3 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV3_SLEW, 0.0);

/**
 * Servo 4 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV4_SLEW, 0.0);

/**
 * Servo 5 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV5_SLEW, 0.0);

/**
 * Servo 6 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV6_SLEW, 0.0);

/**
 * Servo 7 slew rate limit
 *
 * Forces the servo output signal to take at least the configured time (in seconds)
 * to traverse its full range [-100%, 100%].
 * 
 * Zero means that slew rate limiting is disabled.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.05
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_SV7_SLEW, 0.0);

/**
 * Total number of rotors
 *
 * 
 *
 * @group Geometry
 * @value 0 0
 * @value 1 1
 * @value 2 2
 * @value 3 3
 * @value 4 4
 * @value 5 5
 * @value 6 6
 * @value 7 7
 * @value 8 8
 * @value 9 9
 * @value 10 10
 * @value 11 11
 * @value 12 12
 */
PARAM_DEFINE_INT32(CA_ROTOR_COUNT, 0);

/**
 * Position of rotor 0 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_PX, 0.0);

/**
 * Position of rotor 1 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_PX, 0.0);

/**
 * Position of rotor 2 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_PX, 0.0);

/**
 * Position of rotor 3 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_PX, 0.0);

/**
 * Position of rotor 4 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_PX, 0.0);

/**
 * Position of rotor 5 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_PX, 0.0);

/**
 * Position of rotor 6 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_PX, 0.0);

/**
 * Position of rotor 7 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_PX, 0.0);

/**
 * Position of rotor 8 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_PX, 0.0);

/**
 * Position of rotor 9 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_PX, 0.0);

/**
 * Position of rotor 10 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_PX, 0.0);

/**
 * Position of rotor 11 along X body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_PX, 0.0);

/**
 * Position of rotor 0 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_PY, 0.0);

/**
 * Position of rotor 1 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_PY, 0.0);

/**
 * Position of rotor 2 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_PY, 0.0);

/**
 * Position of rotor 3 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_PY, 0.0);

/**
 * Position of rotor 4 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_PY, 0.0);

/**
 * Position of rotor 5 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_PY, 0.0);

/**
 * Position of rotor 6 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_PY, 0.0);

/**
 * Position of rotor 7 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_PY, 0.0);

/**
 * Position of rotor 8 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_PY, 0.0);

/**
 * Position of rotor 9 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_PY, 0.0);

/**
 * Position of rotor 10 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_PY, 0.0);

/**
 * Position of rotor 11 along Y body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_PY, 0.0);

/**
 * Position of rotor 0 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_PZ, 0.0);

/**
 * Position of rotor 1 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_PZ, 0.0);

/**
 * Position of rotor 2 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_PZ, 0.0);

/**
 * Position of rotor 3 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_PZ, 0.0);

/**
 * Position of rotor 4 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_PZ, 0.0);

/**
 * Position of rotor 5 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_PZ, 0.0);

/**
 * Position of rotor 6 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_PZ, 0.0);

/**
 * Position of rotor 7 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_PZ, 0.0);

/**
 * Position of rotor 8 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_PZ, 0.0);

/**
 * Position of rotor 9 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_PZ, 0.0);

/**
 * Position of rotor 10 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_PZ, 0.0);

/**
 * Position of rotor 11 along Z body axis relative to center of gravity
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_PZ, 0.0);

/**
 * Axis of rotor 0 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_AX, 0.0);

/**
 * Axis of rotor 1 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_AX, 0.0);

/**
 * Axis of rotor 2 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_AX, 0.0);

/**
 * Axis of rotor 3 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_AX, 0.0);

/**
 * Axis of rotor 4 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_AX, 0.0);

/**
 * Axis of rotor 5 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_AX, 0.0);

/**
 * Axis of rotor 6 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_AX, 0.0);

/**
 * Axis of rotor 7 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_AX, 0.0);

/**
 * Axis of rotor 8 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_AX, 0.0);

/**
 * Axis of rotor 9 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_AX, 0.0);

/**
 * Axis of rotor 10 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_AX, 0.0);

/**
 * Axis of rotor 11 thrust vector, X body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_AX, 0.0);

/**
 * Axis of rotor 0 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_AY, 0.0);

/**
 * Axis of rotor 1 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_AY, 0.0);

/**
 * Axis of rotor 2 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_AY, 0.0);

/**
 * Axis of rotor 3 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_AY, 0.0);

/**
 * Axis of rotor 4 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_AY, 0.0);

/**
 * Axis of rotor 5 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_AY, 0.0);

/**
 * Axis of rotor 6 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_AY, 0.0);

/**
 * Axis of rotor 7 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_AY, 0.0);

/**
 * Axis of rotor 8 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_AY, 0.0);

/**
 * Axis of rotor 9 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_AY, 0.0);

/**
 * Axis of rotor 10 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_AY, 0.0);

/**
 * Axis of rotor 11 thrust vector, Y body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_AY, 0.0);

/**
 * Axis of rotor 0 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_AZ, -1.0);

/**
 * Axis of rotor 1 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_AZ, -1.0);

/**
 * Axis of rotor 2 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_AZ, -1.0);

/**
 * Axis of rotor 3 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_AZ, -1.0);

/**
 * Axis of rotor 4 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_AZ, -1.0);

/**
 * Axis of rotor 5 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_AZ, -1.0);

/**
 * Axis of rotor 6 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_AZ, -1.0);

/**
 * Axis of rotor 7 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_AZ, -1.0);

/**
 * Axis of rotor 8 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_AZ, -1.0);

/**
 * Axis of rotor 9 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_AZ, -1.0);

/**
 * Axis of rotor 10 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_AZ, -1.0);

/**
 * Axis of rotor 11 thrust vector, Z body axis component
 *
 * Only the direction is considered (the vector is normalized).
 *
 * @group Geometry
 * @decimal 2
 * @increment 0.1
 * @min -100
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_AZ, -1.0);

/**
 * Thrust coefficient of rotor 0
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_CT, 6.5);

/**
 * Thrust coefficient of rotor 1
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_CT, 6.5);

/**
 * Thrust coefficient of rotor 2
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_CT, 6.5);

/**
 * Thrust coefficient of rotor 3
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_CT, 6.5);

/**
 * Thrust coefficient of rotor 4
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_CT, 6.5);

/**
 * Thrust coefficient of rotor 5
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_CT, 6.5);

/**
 * Thrust coefficient of rotor 6
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_CT, 6.5);

/**
 * Thrust coefficient of rotor 7
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_CT, 6.5);

/**
 * Thrust coefficient of rotor 8
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_CT, 6.5);

/**
 * Thrust coefficient of rotor 9
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_CT, 6.5);

/**
 * Thrust coefficient of rotor 10
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_CT, 6.5);

/**
 * Thrust coefficient of rotor 11
 *
 * The thrust coefficient if defined as Thrust = CT * u^2,
 * where u (with value between actuator minimum and maximum)
 * is the output signal sent to the motor controller.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_CT, 6.5);

/**
 * Moment coefficient of rotor 0
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR0_KM, 0.05);

/**
 * Moment coefficient of rotor 1
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR1_KM, 0.05);

/**
 * Moment coefficient of rotor 2
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR2_KM, 0.05);

/**
 * Moment coefficient of rotor 3
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR3_KM, 0.05);

/**
 * Moment coefficient of rotor 4
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR4_KM, 0.05);

/**
 * Moment coefficient of rotor 5
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR5_KM, 0.05);

/**
 * Moment coefficient of rotor 6
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR6_KM, 0.05);

/**
 * Moment coefficient of rotor 7
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR7_KM, 0.05);

/**
 * Moment coefficient of rotor 8
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR8_KM, 0.05);

/**
 * Moment coefficient of rotor 9
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR9_KM, 0.05);

/**
 * Moment coefficient of rotor 10
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR10_KM, 0.05);

/**
 * Moment coefficient of rotor 11
 *
 * The moment coefficient if defined as Torque = KM * Thrust.
 * 
 * Use a positive value for a rotor with CCW rotation.
 * Use a negative value for a rotor with CW rotation.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.01
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_ROTOR11_KM, 0.05);

/**
 * Rotor 0 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR0_TILT, 0);

/**
 * Rotor 1 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR1_TILT, 0);

/**
 * Rotor 2 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR2_TILT, 0);

/**
 * Rotor 3 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR3_TILT, 0);

/**
 * Rotor 4 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR4_TILT, 0);

/**
 * Rotor 5 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR5_TILT, 0);

/**
 * Rotor 6 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR6_TILT, 0);

/**
 * Rotor 7 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR7_TILT, 0);

/**
 * Rotor 8 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR8_TILT, 0);

/**
 * Rotor 9 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR9_TILT, 0);

/**
 * Rotor 10 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR10_TILT, 0);

/**
 * Rotor 11 tilt assignment
 *
 * If not set to None, this motor is tilted by the configured tilt servo.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Tilt 1
 * @value 2 Tilt 2
 * @value 3 Tilt 3
 * @value 4 Tilt 4
 */
PARAM_DEFINE_INT32(CA_ROTOR11_TILT, 0);

/**
 * Total number of Control Surfaces
 *
 * 
 *
 * @group Geometry
 * @value 0 0
 * @value 1 1
 * @value 2 2
 * @value 3 3
 * @value 4 4
 * @value 5 5
 * @value 6 6
 * @value 7 7
 * @value 8 8
 */
PARAM_DEFINE_INT32(CA_SV_CS_COUNT, 0);

/**
 * Control Surface 0 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS0_TYPE, 0);

/**
 * Control Surface 1 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS1_TYPE, 0);

/**
 * Control Surface 2 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS2_TYPE, 0);

/**
 * Control Surface 3 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS3_TYPE, 0);

/**
 * Control Surface 4 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS4_TYPE, 0);

/**
 * Control Surface 5 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS5_TYPE, 0);

/**
 * Control Surface 6 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS6_TYPE, 0);

/**
 * Control Surface 7 type
 *
 * 
 *
 * @group Geometry
 * @value 0 (Not set)
 * @value 1 Left Aileron
 * @value 2 Right Aileron
 * @value 3 Elevator
 * @value 4 Rudder
 * @value 5 Left Elevon
 * @value 6 Right Elevon
 * @value 7 Left V-Tail
 * @value 8 Right V-Tail
 * @value 9 Left Flap
 * @value 10 Right Flap
 * @value 11 Airbrake
 * @value 12 Custom
 * @value 13 Left A-tail
 * @value 14 Right A-tail
 * @value 15 Single Channel Aileron
 * @value 16 Steering Wheel
 * @value 17 Left Spoiler
 * @value 18 Right Spoiler
 */
PARAM_DEFINE_INT32(CA_SV_CS7_TYPE, 0);

/**
 * Control Surface 0 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_TRQ_R, 0.0);

/**
 * Control Surface 1 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_TRQ_R, 0.0);

/**
 * Control Surface 2 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_TRQ_R, 0.0);

/**
 * Control Surface 3 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_TRQ_R, 0.0);

/**
 * Control Surface 4 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_TRQ_R, 0.0);

/**
 * Control Surface 5 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_TRQ_R, 0.0);

/**
 * Control Surface 6 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_TRQ_R, 0.0);

/**
 * Control Surface 7 roll torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_TRQ_R, 0.0);

/**
 * Control Surface 0 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_TRQ_P, 0.0);

/**
 * Control Surface 1 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_TRQ_P, 0.0);

/**
 * Control Surface 2 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_TRQ_P, 0.0);

/**
 * Control Surface 3 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_TRQ_P, 0.0);

/**
 * Control Surface 4 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_TRQ_P, 0.0);

/**
 * Control Surface 5 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_TRQ_P, 0.0);

/**
 * Control Surface 6 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_TRQ_P, 0.0);

/**
 * Control Surface 7 pitch torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_TRQ_P, 0.0);

/**
 * Control Surface 0 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_TRQ_Y, 0.0);

/**
 * Control Surface 1 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_TRQ_Y, 0.0);

/**
 * Control Surface 2 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_TRQ_Y, 0.0);

/**
 * Control Surface 3 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_TRQ_Y, 0.0);

/**
 * Control Surface 4 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_TRQ_Y, 0.0);

/**
 * Control Surface 5 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_TRQ_Y, 0.0);

/**
 * Control Surface 6 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_TRQ_Y, 0.0);

/**
 * Control Surface 7 yaw torque scaling
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_TRQ_Y, 0.0);

/**
 * Control Surface 0 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_TRIM, 0.0);

/**
 * Control Surface 1 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_TRIM, 0.0);

/**
 * Control Surface 2 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_TRIM, 0.0);

/**
 * Control Surface 3 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_TRIM, 0.0);

/**
 * Control Surface 4 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_TRIM, 0.0);

/**
 * Control Surface 5 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_TRIM, 0.0);

/**
 * Control Surface 6 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_TRIM, 0.0);

/**
 * Control Surface 7 trim
 *
 * Can be used to add an offset to the servo control.
 * 
 * NOTE: Do not use for PWM servos. Use the PWM CENTER parameters instead (e.g., PWM_MAIN_CENT, PWM_AUX_CENT) instead.
 * This parameter can only be set if all PWM Center parameters are set to default.
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_TRIM, 0.0);

/**
 * Control Surface 0 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_FLAP, 0);

/**
 * Control Surface 1 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_FLAP, 0);

/**
 * Control Surface 2 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_FLAP, 0);

/**
 * Control Surface 3 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_FLAP, 0);

/**
 * Control Surface 4 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_FLAP, 0);

/**
 * Control Surface 5 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_FLAP, 0);

/**
 * Control Surface 6 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_FLAP, 0);

/**
 * Control Surface 7 configuration as flap
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_FLAP, 0);

/**
 * Control Surface slew rate for normalized flaps setpoint
 *
 * 
 *
 * @group Geometry
 * @decimal 1
 * @min 0.0
 * @max 5.0
 */
PARAM_DEFINE_FLOAT(CA_SV_FLAP_SLEW, 0.5);

/**
 * Control Surface 0 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS0_SPOIL, 0);

/**
 * Control Surface 1 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS1_SPOIL, 0);

/**
 * Control Surface 2 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS2_SPOIL, 0);

/**
 * Control Surface 3 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS3_SPOIL, 0);

/**
 * Control Surface 4 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS4_SPOIL, 0);

/**
 * Control Surface 5 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS5_SPOIL, 0);

/**
 * Control Surface 6 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS6_SPOIL, 0);

/**
 * Control Surface 7 configuration as spoiler
 *
 * 
 *
 * @group Geometry
 * @decimal 2
 * @min -1.0
 * @max 1.0
 */
PARAM_DEFINE_FLOAT(CA_SV_CS7_SPOIL, 0);

/**
 * Control surface launch lock enabled
 *
 * If actuator launch lock is enabled, this surface is kept at the disarmed value.
 *
 * @group Geometry
 * @bit 0 Control Surface 1
 * @bit 1 Control Surface 2
 * @bit 2 Control Surface 3
 * @bit 3 Control Surface 4
 * @bit 4 Control Surface 5
 * @bit 5 Control Surface 6
 * @bit 6 Control Surface 7
 * @bit 7 Control Surface 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(CA_CS_LAUN_LK, 0);

/**
 * Total number of Tilt Servos
 *
 * 
 *
 * @group Geometry
 * @value 0 0
 * @value 1 1
 * @value 2 2
 * @value 3 3
 * @value 4 4
 */
PARAM_DEFINE_INT32(CA_SV_TL_COUNT, 0);

/**
 * Tilt 0 is used for control
 *
 * Define if this servo is used for additional control.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Yaw
 * @value 2 Pitch
 * @value 3 Yaw and Pitch
 */
PARAM_DEFINE_INT32(CA_SV_TL0_CT, 1);

/**
 * Tilt 1 is used for control
 *
 * Define if this servo is used for additional control.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Yaw
 * @value 2 Pitch
 * @value 3 Yaw and Pitch
 */
PARAM_DEFINE_INT32(CA_SV_TL1_CT, 1);

/**
 * Tilt 2 is used for control
 *
 * Define if this servo is used for additional control.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Yaw
 * @value 2 Pitch
 * @value 3 Yaw and Pitch
 */
PARAM_DEFINE_INT32(CA_SV_TL2_CT, 1);

/**
 * Tilt 3 is used for control
 *
 * Define if this servo is used for additional control.
 *
 * @group Geometry
 * @value 0 None
 * @value 1 Yaw
 * @value 2 Pitch
 * @value 3 Yaw and Pitch
 */
PARAM_DEFINE_INT32(CA_SV_TL3_CT, 1);

/**
 * Tilt Servo 0 Tilt Angle at Minimum
 *
 * Defines the tilt angle when the servo is at the minimum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL0_MINA, 0.0);

/**
 * Tilt Servo 1 Tilt Angle at Minimum
 *
 * Defines the tilt angle when the servo is at the minimum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL1_MINA, 0.0);

/**
 * Tilt Servo 2 Tilt Angle at Minimum
 *
 * Defines the tilt angle when the servo is at the minimum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL2_MINA, 0.0);

/**
 * Tilt Servo 3 Tilt Angle at Minimum
 *
 * Defines the tilt angle when the servo is at the minimum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL3_MINA, 0.0);

/**
 * Tilt Servo 0 Tilt Angle at Maximum
 *
 * Defines the tilt angle when the servo is at the maximum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL0_MAXA, 90.0);

/**
 * Tilt Servo 1 Tilt Angle at Maximum
 *
 * Defines the tilt angle when the servo is at the maximum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL1_MAXA, 90.0);

/**
 * Tilt Servo 2 Tilt Angle at Maximum
 *
 * Defines the tilt angle when the servo is at the maximum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL2_MAXA, 90.0);

/**
 * Tilt Servo 3 Tilt Angle at Maximum
 *
 * Defines the tilt angle when the servo is at the maximum.
 * An angle of zero means upwards.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -90.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SV_TL3_MAXA, 90.0);

/**
 * Tilt Servo 0 Tilt Direction
 *
 * Defines the direction the servo tilts towards when moving towards the maximum tilt angle.
 * For example if the minimum tilt angle is -90, the maximum 90, and the direction 'Towards Front',
 * the motor axis aligns with the XZ-plane, points towards -X at the minimum and +X at the maximum tilt.
 * 
 *
 * @group Geometry
 * @value 0 Towards Front
 * @value 90 Towards Right
 * @min 0
 * @max 359
 */
PARAM_DEFINE_INT32(CA_SV_TL0_TD, 0);

/**
 * Tilt Servo 1 Tilt Direction
 *
 * Defines the direction the servo tilts towards when moving towards the maximum tilt angle.
 * For example if the minimum tilt angle is -90, the maximum 90, and the direction 'Towards Front',
 * the motor axis aligns with the XZ-plane, points towards -X at the minimum and +X at the maximum tilt.
 * 
 *
 * @group Geometry
 * @value 0 Towards Front
 * @value 90 Towards Right
 * @min 0
 * @max 359
 */
PARAM_DEFINE_INT32(CA_SV_TL1_TD, 0);

/**
 * Tilt Servo 2 Tilt Direction
 *
 * Defines the direction the servo tilts towards when moving towards the maximum tilt angle.
 * For example if the minimum tilt angle is -90, the maximum 90, and the direction 'Towards Front',
 * the motor axis aligns with the XZ-plane, points towards -X at the minimum and +X at the maximum tilt.
 * 
 *
 * @group Geometry
 * @value 0 Towards Front
 * @value 90 Towards Right
 * @min 0
 * @max 359
 */
PARAM_DEFINE_INT32(CA_SV_TL2_TD, 0);

/**
 * Tilt Servo 3 Tilt Direction
 *
 * Defines the direction the servo tilts towards when moving towards the maximum tilt angle.
 * For example if the minimum tilt angle is -90, the maximum 90, and the direction 'Towards Front',
 * the motor axis aligns with the XZ-plane, points towards -X at the minimum and +X at the maximum tilt.
 * 
 *
 * @group Geometry
 * @value 0 Towards Front
 * @value 90 Towards Right
 * @min 0
 * @max 359
 */
PARAM_DEFINE_INT32(CA_SV_TL3_TD, 0);

/**
 * Number of swash plates servos
 *
 * 
 *
 * @group Geometry
 * @value 2 2
 * @value 3 3
 * @value 4 4
 */
PARAM_DEFINE_INT32(CA_SP0_COUNT, 3);

/**
 * Angle for swash plate servo 0
 *
 * The angle is measured clockwise (as seen from top), with 0 pointing forwards (X axis).
 * 
 *
 * @group Geometry
 * @decimal 0
 * @increment 10
 * @min 0
 * @max 360
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SP0_ANG0, 0);

/**
 * Angle for swash plate servo 1
 *
 * The angle is measured clockwise (as seen from top), with 0 pointing forwards (X axis).
 * 
 *
 * @group Geometry
 * @decimal 0
 * @increment 10
 * @min 0
 * @max 360
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SP0_ANG1, 140);

/**
 * Angle for swash plate servo 2
 *
 * The angle is measured clockwise (as seen from top), with 0 pointing forwards (X axis).
 * 
 *
 * @group Geometry
 * @decimal 0
 * @increment 10
 * @min 0
 * @max 360
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SP0_ANG2, 220);

/**
 * Angle for swash plate servo 3
 *
 * The angle is measured clockwise (as seen from top), with 0 pointing forwards (X axis).
 * 
 *
 * @group Geometry
 * @decimal 0
 * @increment 10
 * @min 0
 * @max 360
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_SP0_ANG3, 0);

/**
 * Arm length for swash plate servo 0
 *
 * This is relative to the other arm lengths.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_SP0_ARM_L0, 1.0);

/**
 * Arm length for swash plate servo 1
 *
 * This is relative to the other arm lengths.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_SP0_ARM_L1, 1.0);

/**
 * Arm length for swash plate servo 2
 *
 * This is relative to the other arm lengths.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_SP0_ARM_L2, 1.0);

/**
 * Arm length for swash plate servo 3
 *
 * This is relative to the other arm lengths.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_SP0_ARM_L3, 1.0);

/**
 * Throttle curve at position 0
 *
 * Defines the output throttle at the interval position 0.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_THR_C0, 1);

/**
 * Throttle curve at position 1
 *
 * Defines the output throttle at the interval position 1.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_THR_C1, 1);

/**
 * Throttle curve at position 2
 *
 * Defines the output throttle at the interval position 2.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_THR_C2, 1);

/**
 * Throttle curve at position 3
 *
 * Defines the output throttle at the interval position 3.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_THR_C3, 1);

/**
 * Throttle curve at position 4
 *
 * Defines the output throttle at the interval position 4.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_THR_C4, 1);

/**
 * Collective pitch curve at position 0
 *
 * Defines the collective pitch at the interval position 0 for a given thrust setpoint.
 * 
 * Use negative values if the swash plate needs to move down to provide upwards thrust.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_PITCH_C0, -0.05);

/**
 * Collective pitch curve at position 1
 *
 * Defines the collective pitch at the interval position 1 for a given thrust setpoint.
 * 
 * Use negative values if the swash plate needs to move down to provide upwards thrust.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_PITCH_C1, 0.0725);

/**
 * Collective pitch curve at position 2
 *
 * Defines the collective pitch at the interval position 2 for a given thrust setpoint.
 * 
 * Use negative values if the swash plate needs to move down to provide upwards thrust.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_PITCH_C2, 0.2);

/**
 * Collective pitch curve at position 3
 *
 * Defines the collective pitch at the interval position 3 for a given thrust setpoint.
 * 
 * Use negative values if the swash plate needs to move down to provide upwards thrust.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_PITCH_C3, 0.325);

/**
 * Collective pitch curve at position 4
 *
 * Defines the collective pitch at the interval position 4 for a given thrust setpoint.
 * 
 * Use negative values if the swash plate needs to move down to provide upwards thrust.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -1
 * @max 1
 */
PARAM_DEFINE_FLOAT(CA_HELI_PITCH_C4, 0.45);

/**
 * Scale for yaw compensation based on collective pitch
 *
 * This allows to add a proportional factor of the collective pitch command to the yaw command.
 * A negative value is needed when positive thrust of the tail rotor rotates the vehicle opposite to the main rotor turn direction.
 * 
 * tail_output += CA_HELI_YAW_CP_S * abs(collective_pitch - CA_HELI_YAW_CP_O)
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -2
 * @max 2
 */
PARAM_DEFINE_FLOAT(CA_HELI_YAW_CP_S, 0.0);

/**
 * Offset for yaw compensation based on collective pitch
 *
 * This allows to specify which collective pitch command results in the least amount of rotor drag.
 * This is used to increase the accuracy of the yaw drag torque compensation based on collective pitch
 * by aligning the lowest rotor drag with zero compensation.
 * For symmetric profile blades this is the command that results in exactly 0° collective blade angle.
 * For lift profile blades this is typically a command resulting in slightly negative collective blade angle.
 * 
 * tail_output += CA_HELI_YAW_CP_S * abs(collective_pitch - CA_HELI_YAW_CP_O)
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -2
 * @max 2
 */
PARAM_DEFINE_FLOAT(CA_HELI_YAW_CP_O, 0.0);

/**
 * Scale for yaw compensation based on throttle
 *
 * This allows to add a proportional factor of the throttle command to the yaw command.
 * A negative value is needed when positive thrust of the tail rotor rotates the vehicle opposite to the main rotor turn direction.
 * 
 * tail_output += CA_HELI_YAW_TH_S * throttle
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min -2
 * @max 2
 */
PARAM_DEFINE_FLOAT(CA_HELI_YAW_TH_S, 0.0);

/**
 * Main rotor turns counter-clockwise
 *
 * Default configuration is for a clockwise turning main rotor and
 * positive thrust of the tail rotor is expected to rotate the vehicle clockwise.
 * Set this parameter to true if the tail rotor provides thrust in counter-clockwise direction
 * which is mostly the case when the main rotor turns counter-clockwise.
 * 
 *
 * @group Geometry
 * @boolean
 */
PARAM_DEFINE_INT32(CA_HELI_YAW_CCW, 0);

/**
 * Setpoint for main rotor rpm
 *
 * Requires rpm feedback for the controller.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @increment 1
 * @min 100
 * @max 10000
 */
PARAM_DEFINE_FLOAT(CA_HELI_RPM_SP, 1500);

/**
 * Proportional gain for rpm control
 *
 * Ratio between rpm error devided by 1000 to how much normalized output gets added to correct for it.
 * 
 * motor_command = throttle_curve + CA_HELI_RPM_P * (rpm_setpoint - rpm_measurement) / 1000
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_HELI_RPM_P, 0.0);

/**
 * Integral gain for rpm control
 *
 * Same definition as the proportional gain but for integral.
 * 
 *
 * @group Geometry
 * @decimal 3
 * @increment 0.1
 * @min 0
 * @max 10
 */
PARAM_DEFINE_FLOAT(CA_HELI_RPM_I, 0.0);

/**
 * Throw angle of swashplate servo at maximum commands for linearization
 *
 * Used to linearize mechanical output of swashplate servos to avoid axis coupling and binding with 4 servo redundancy.
 * This requires a symmetric setup where the servo horn is exactly centered with a 0 command.
 * Setting to zero disables feature.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 0.1
 * @min 0
 * @max 75
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CA_MAX_SVO_THROW, 0.0);

/**
 * Motor failure handling mode
 *
 * This is used to specify how to handle motor failures
 * reported by failure detector.
 * 
 *
 * @group Geometry
 * @value 0 Ignore
 * @value 1 Remove first failed motor from effectiveness
 */
PARAM_DEFINE_INT32(CA_FAILURE_MODE, 0);

/**
 * Ice shedding cycle period
 *
 * Ice shedding prevents ice buildup in VTOL aircraft motors by periodically spinning inactive rotors.
 * When enabled (period > 0), every cycle lasts for the defined period and includes a 2-second spin at 0.01 motor output.
 * If period <= 0, the feature is disabled.
 * 
 *
 * @group Geometry
 * @decimal 1
 * @increment 0.1
 * @min 0.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(CA_ICE_PERIOD, 0.0);

/**
 * EKF2 enable
 *
 * 
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_EN, 1);

/**
 * Verbose logging
 *
 * 
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_LOG_VERBOSE, 1);

/**
 * EKF prediction period
 *
 * EKF prediction period in microseconds. This should ideally be an integer multiple of the IMU time delta. Actual filter update will be an integer multiple of IMU update.
 *
 * @group EKF2
 * @min 1000
 * @max 20000
 * @unit us
 */
PARAM_DEFINE_INT32(EKF2_PREDICT_US, 10000);

/**
 * Maximum delay of all the aiding sensors
 *
 * Defines the delay between the current time and the delayed-time horizon. This value should be at least as large as the largest EKF2_XXX_DELAY parameter.
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 1000
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_DELAY_MAX, 200);

/**
 * 1-sigma tilt angle uncertainty after gravity vector alignment
 *
 * 
 *
 * @group EKF2
 * @decimal 3
 * @min 0.0
 * @max 0.5
 * @unit rad
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_ANGERR_INIT, 0.1);

/**
 * Gate size for heading fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_HDG_GATE, 2.6);

/**
 * Measurement noise for magnetic heading fusion
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @max 1.0
 * @unit rad
 */
PARAM_DEFINE_FLOAT(EKF2_HEAD_NOISE, 0.3);

/**
 * Measurement noise for non-aiding position hold
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0.5
 * @max 50.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_NOAID_NOISE, 10.0);

/**
 * Maximum inertial dead-reckoning time
 *
 * Maximum lapsed time from last fusion of measurements that constrain velocity drift before the EKF will report the horizontal nav solution as invalid
 *
 * @group EKF2
 * @min 500000
 * @max 10000000
 * @unit us
 */
PARAM_DEFINE_INT32(EKF2_NOAID_TOUT, 5000000);

/**
 * Determines the reference source of height data used by the EKF
 *
 * When multiple height sources are enabled at the same time, the height estimate will always converge towards the reference height source selected by this parameter. The range sensor and vision options should only be used when for operation over a flat surface as the local NED origin will move up and down with ground level.
 * If GPS is set as reference and EKF2_GPS_CTRL is not 0, the GPS altitude is still used to initiaize the bias of the other height sensors, regardless of the altitude fusion bit in EKF2_GPS_CTRL.
 *
 * @group EKF2
 * @value 0 Barometric pressure
 * @value 1 GPS
 * @value 2 Range sensor
 * @value 3 Vision
 * @reboot_required True
 */
PARAM_DEFINE_INT32(EKF2_HGT_REF, 1);

/**
 * IMU control
 *
 * 
 *
 * @group EKF2
 * @bit 0 Gyro Bias
 * @bit 1 Accel Bias
 * @bit 2 Gravity vector fusion
 * @min 0
 * @max 7
 * @min 0
 * @max 7
 */
PARAM_DEFINE_INT32(EKF2_IMU_CTRL, 7);

/**
 * Rate gyro noise for covariance prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 4
 * @min 0.0001
 * @max 0.1
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(EKF2_GYR_NOISE, 0.015);

/**
 * Accelerometer noise for covariance prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @max 1.0
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_ACC_NOISE, 0.35);

/**
 * X position of IMU in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_IMU_POS_X, 0.0);

/**
 * Y position of IMU in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_IMU_POS_Y, 0.0);

/**
 * Z position of IMU in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_IMU_POS_Z, 0.0);

/**
 * Time constant of the velocity output prediction and smoothing filter
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @max 1.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(EKF2_TAU_VEL, 0.25);

/**
 * Output predictor position time constant
 *
 * Controls how tightly the output track the EKF states
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 1.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(EKF2_TAU_POS, 0.25);

/**
 * Velocity limit
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @max 299792458
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_VEL_LIM, 100);

/**
 * Enable constant position fusion during engine warmup
 *
 * When enabled, constant position fusion is enabled when the vehicle is landed and armed. This is intended for IC engine warmup (e.g., fuel engines on catapult) to allow mode transitions to auto/takeoff despite vibrations from running engines.
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_ENGINE_WRM, 0);

/**
 * 1-sigma IMU accelerometer switch-on bias
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @max 0.5
 * @unit m/s^2
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_ABIAS_INIT, 0.2);

/**
 * Process noise for IMU accelerometer bias prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 6
 * @min 0.0
 * @max 0.01
 * @unit m/s^3
 */
PARAM_DEFINE_FLOAT(EKF2_ACC_B_NOISE, 0.003);

/**
 * Accelerometer bias learning limit
 *
 * The ekf accel bias states will be limited to within a range equivalent to +- of this value.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @max 0.8
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_ABL_LIM, 0.4);

/**
 * Maximum IMU accel magnitude that allows IMU bias learning
 *
 * If the magnitude of the IMU accelerometer vector exceeds this value, the EKF accel bias state estimation will be inhibited. This reduces the adverse effect of high manoeuvre accelerations and IMU nonlinerity and scale factor errors on the accel bias estimates.
 *
 * @group EKF2
 * @decimal 1
 * @min 20.0
 * @max 200.0
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_ABL_ACCLIM, 25.0);

/**
 * Maximum IMU gyro angular rate magnitude that allows IMU bias learning
 *
 * If the magnitude of the IMU angular rate vector exceeds this value, the EKF accel bias state estimation will be inhibited. This reduces the adverse effect of rapid rotation rates and associated errors on the accel bias estimates.
 *
 * @group EKF2
 * @decimal 1
 * @min 2.0
 * @max 20.0
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(EKF2_ABL_GYRLIM, 3.0);

/**
 * Accel bias learning inhibit time constant
 *
 * The vector magnitude of angular rate and acceleration used to check if learning should be inhibited has a peak hold filter applied to it with an exponential decay. This parameter controls the time constant of the decay.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 1.0
 * @unit s
 */
PARAM_DEFINE_FLOAT(EKF2_ABL_TAU, 0.5);

/**
 * Airspeed fusion threshold
 *
 * Airspeed data is fused for wind estimation if above this threshold. Set to 0 to disable airspeed fusion. For reliable wind estimation both sideslip (see EKF2_FUSE_BETA) and airspeed fusion should be enabled. Only applies to fixed-wing vehicles (or VTOLs in fixed-wing mode).
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_ARSP_THR, 0.0);

/**
 * Airspeed measurement delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_ASP_DELAY, 100);

/**
 * Gate size for TAS fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_TAS_GATE, 5.0);

/**
 * Measurement noise for airspeed fusion
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0.5
 * @max 5.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_EAS_NOISE, 1.4);

/**
 * Auxiliary global position sensor 0 ID
 *
 * Sensor ID for slot 0. Set to 0 to disable this slot.
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_AGP0_ID, 0);

/**
 * Auxiliary global position sensor 1 ID
 *
 * Sensor ID for slot 1. Set to 0 to disable this slot.
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_AGP1_ID, 0);

/**
 * Auxiliary global position sensor 2 ID
 *
 * Sensor ID for slot 2. Set to 0 to disable this slot.
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_AGP2_ID, 0);

/**
 * Auxiliary global position sensor 3 ID
 *
 * Sensor ID for slot 3. Set to 0 to disable this slot.
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_AGP3_ID, 0);

/**
 * Auxiliary global position sensor 0 aiding
 *
 * Set bits in the following positions to enable: 0 : Horizontal position fusion 1 : Vertical position fusion
 *
 * @group EKF2
 * @bit 0 Horizontal position
 * @bit 1 Vertical position
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(EKF2_AGP0_CTRL, 0);

/**
 * Auxiliary global position sensor 1 aiding
 *
 * Set bits in the following positions to enable: 0 : Horizontal position fusion 1 : Vertical position fusion
 *
 * @group EKF2
 * @bit 0 Horizontal position
 * @bit 1 Vertical position
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(EKF2_AGP1_CTRL, 0);

/**
 * Auxiliary global position sensor 2 aiding
 *
 * Set bits in the following positions to enable: 0 : Horizontal position fusion 1 : Vertical position fusion
 *
 * @group EKF2
 * @bit 0 Horizontal position
 * @bit 1 Vertical position
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(EKF2_AGP2_CTRL, 0);

/**
 * Auxiliary global position sensor 3 aiding
 *
 * Set bits in the following positions to enable: 0 : Horizontal position fusion 1 : Vertical position fusion
 *
 * @group EKF2
 * @bit 0 Horizontal position
 * @bit 1 Vertical position
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 */
PARAM_DEFINE_INT32(EKF2_AGP3_CTRL, 0);

/**
 * Fusion reset mode for sensor 0
 *
 * Automatic: reset on fusion timeout if no other source of position is available Dead-reckoning: reset on fusion timeout if no source of velocity is available
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Dead-reckoning
 */
PARAM_DEFINE_INT32(EKF2_AGP0_MODE, 0);

/**
 * Fusion reset mode for sensor 1
 *
 * Automatic: reset on fusion timeout if no other source of position is available Dead-reckoning: reset on fusion timeout if no source of velocity is available
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Dead-reckoning
 */
PARAM_DEFINE_INT32(EKF2_AGP1_MODE, 0);

/**
 * Fusion reset mode for sensor 2
 *
 * Automatic: reset on fusion timeout if no other source of position is available Dead-reckoning: reset on fusion timeout if no source of velocity is available
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Dead-reckoning
 */
PARAM_DEFINE_INT32(EKF2_AGP2_MODE, 0);

/**
 * Fusion reset mode for sensor 3
 *
 * Automatic: reset on fusion timeout if no other source of position is available Dead-reckoning: reset on fusion timeout if no source of velocity is available
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Dead-reckoning
 */
PARAM_DEFINE_INT32(EKF2_AGP3_MODE, 0);

/**
 * Auxiliary global position sensor 0 delay (to IMU)
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 1000
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_AGP0_DELAY, 0);

/**
 * Auxiliary global position sensor 1 delay (to IMU)
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 1000
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_AGP1_DELAY, 0);

/**
 * Auxiliary global position sensor 2 delay (to IMU)
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 1000
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_AGP2_DELAY, 0);

/**
 * Auxiliary global position sensor 3 delay (to IMU)
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 1000
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_AGP3_DELAY, 0);

/**
 * Measurement noise for auxiliary global position sensor 0
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_AGP0_NOISE, 1.0);

/**
 * Measurement noise for auxiliary global position sensor 1
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_AGP1_NOISE, 1.0);

/**
 * Measurement noise for auxiliary global position sensor 2
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_AGP2_NOISE, 1.0);

/**
 * Measurement noise for auxiliary global position sensor 3
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_AGP3_NOISE, 1.0);

/**
 * Gate size for auxiliary global position sensor 0 fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_AGP0_GATE, 3.0);

/**
 * Gate size for auxiliary global position sensor 1 fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_AGP1_GATE, 3.0);

/**
 * Gate size for auxiliary global position sensor 2 fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_AGP2_GATE, 3.0);

/**
 * Gate size for auxiliary global position sensor 3 fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_AGP3_GATE, 3.0);

/**
 * Auxiliary Velocity Estimate delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_AVEL_DELAY, 5);

/**
 * Barometric sensor height aiding
 *
 * If this parameter is enabled then the estimator will make use of the barometric height measurements to estimate its height in addition to other height sources (if activated).
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_BARO_CTRL, 1);

/**
 * Barometer measurement delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_BARO_DELAY, 0);

/**
 * Gate size for barometric and GPS height fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_BARO_GATE, 5.0);

/**
 * Measurement noise for barometric altitude
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @max 15.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_BARO_NOISE, 3.5);

/**
 * Baro deadzone range for height fusion
 *
 * Sets the value of deadzone applied to negative baro innovations. Deadzone is enabled when EKF2_GND_EFF_DZ > 0.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 10.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GND_EFF_DZ, 4.0);

/**
 * Height above ground level for ground effect zone
 *
 * Sets the maximum distance to the ground level where negative baro innovations are expected.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 5.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GND_MAX_HGT, 0.5);

/**
 * Static pressure position error coefficient for the positive X axis
 *
 * This is the ratio of static pressure error to dynamic pressure generated by a positive wind relative velocity along the X body axis. If the baro height estimate rises during forward flight, then this will be a negative number.
 *
 * @group EKF2
 * @decimal 2
 * @min -0.5
 * @max 0.5
 */
PARAM_DEFINE_FLOAT(EKF2_PCOEF_XP, 0.0);

/**
 * Static pressure position error coefficient for the negative X axis
 *
 * This is the ratio of static pressure error to dynamic pressure generated by a negative wind relative velocity along the X body axis. If the baro height estimate rises during backwards flight, then this will be a negative number.
 *
 * @group EKF2
 * @decimal 2
 * @min -0.5
 * @max 0.5
 */
PARAM_DEFINE_FLOAT(EKF2_PCOEF_XN, 0.0);

/**
 * Pressure position error coefficient for the positive Y axis
 *
 * This is the ratio of static pressure error to dynamic pressure generated by a wind relative velocity along the positive Y (RH) body axis. If the baro height estimate rises during sideways flight to the right, then this will be a negative number.
 *
 * @group EKF2
 * @decimal 2
 * @min -0.5
 * @max 0.5
 */
PARAM_DEFINE_FLOAT(EKF2_PCOEF_YP, 0.0);

/**
 * Pressure position error coefficient for the negative Y axis
 *
 * This is the ratio of static pressure error to dynamic pressure generated by a wind relative velocity along the negative Y (LH) body axis. If the baro height estimate rises during sideways flight to the left, then this will be a negative number.
 *
 * @group EKF2
 * @decimal 2
 * @min -0.5
 * @max 0.5
 */
PARAM_DEFINE_FLOAT(EKF2_PCOEF_YN, 0.0);

/**
 * Static pressure position error coefficient for the Z axis
 *
 * This is the ratio of static pressure error to dynamic pressure generated by a wind relative velocity along the Z body axis.
 *
 * @group EKF2
 * @decimal 2
 * @min -0.5
 * @max 0.5
 */
PARAM_DEFINE_FLOAT(EKF2_PCOEF_Z, 0.0);

/**
 * Maximum airspeed used for baro static pressure compensation
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 5.0
 * @max 50.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_ASPD_MAX, 20.0);

/**
 * Multirotor wind estimation selection
 *
 * Activate wind speed estimation using specific-force measurements and a drag model defined by EKF2_BCOEF_[XY] and EKF2_MCOEF. Only use on vehicles that have their thrust aligned with the Z axis and no thrust in the XY plane.
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_DRAG_CTRL, 0);

/**
 * Specific drag force observation noise variance
 *
 * Used by the multi-rotor specific drag force model. Increasing this makes the multi-rotor wind estimates adjust more slowly.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.5
 * @max 10.0
 * @unit (m/s^2)^2
 */
PARAM_DEFINE_FLOAT(EKF2_DRAG_NOISE, 2.5);

/**
 * X-axis ballistic coefficient used for multi-rotor wind estimation
 *
 * This parameter controls the prediction of drag produced by bluff body drag along the forward/reverse axis when flying a multi-copter which enables estimation of wind drift when enabled by the EKF2_DRAG_CTRL parameter. The drag produced by this effect scales with speed squared. The predicted drag from the rotors is specified separately by the EKF2_MCOEF parameter. Set this parameter to zero to turn off the bluff body drag model for this axis.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 200.0
 * @unit kg/m^2
 */
PARAM_DEFINE_FLOAT(EKF2_BCOEF_X, 100.0);

/**
 * Y-axis ballistic coefficient used for multi-rotor wind estimation
 *
 * This parameter controls the prediction of drag produced by bluff body drag along the right/left axis when flying a multi-copter, which enables estimation of wind drift when enabled by the EKF2_DRAG_CTRL parameter. The drag produced by this effect scales with speed squared. The predicted drag from the rotors is specified separately by the EKF2_MCOEF parameter. Set this parameter to zero to turn off the bluff body drag model for this axis.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 200.0
 * @unit kg/m^2
 */
PARAM_DEFINE_FLOAT(EKF2_BCOEF_Y, 100.0);

/**
 * Propeller momentum drag coefficient for multi-rotor wind estimation
 *
 * This parameter controls the prediction of drag produced by the propellers when flying a multi-copter, which enables estimation of wind drift when enabled by the EKF2_DRAG_CTRL parameter. The drag produced by this effect scales with speed not speed squared and is produced because some of the air velocity normal to the propeller axis of rotation is lost when passing through the rotor disc. This  changes the momentum of the flow which creates a drag reaction force. When comparing un-ducted propellers of the same diameter, the effect is roughly proportional to the area of the propeller blades when viewed side on and changes with propeller selection. Momentum drag is significantly higher for ducted rotors. To account for the drag produced by the body which scales with speed squared, see documentation for the EKF2_BCOEF_X and EKF2_BCOEF_Y parameters. Set this parameter to zero to turn off the momentum drag model for both axis.
 *
 * @group EKF2
 * @decimal 2
 * @min 0
 * @max 1.0
 * @unit 1/s
 */
PARAM_DEFINE_FLOAT(EKF2_MCOEF, 0.15);

/**
 * External vision (EV) sensor aiding
 *
 * Set bits in the following positions to enable: 0 : Horizontal position fusion 1 : Vertical position fusion 2 : 3D velocity fusion 3 : Yaw
 *
 * @group EKF2
 * @bit 0 Horizontal position
 * @bit 1 Vertical position
 * @bit 2 3D velocity
 * @bit 3 Yaw
 * @min 0
 * @max 15
 * @min 0
 * @max 15
 */
PARAM_DEFINE_INT32(EKF2_EV_CTRL, 0);

/**
 * Vision Position Estimator delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_EV_DELAY, 0);

/**
 * External vision (EV) noise mode
 *
 * If set to 0 (default) the measurement noise is taken from the vision message and the EV noise parameters are used as a lower bound. If set to 1 the observation noise is set from the parameters directly,
 *
 * @group EKF2
 * @value 0 EV reported variance (parameter lower bound)
 * @value 1 EV noise parameters
 */
PARAM_DEFINE_INT32(EKF2_EV_NOISE_MD, 0);

/**
 * External vision (EV) minimum quality (optional)
 *
 * External vision will only be started and fused if the quality metric is above this threshold. The quality metric is a completely optional field provided by some VIO systems.
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 100
 */
PARAM_DEFINE_INT32(EKF2_EV_QMIN, 0);

/**
 * Measurement noise for vision angle measurements
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.05
 * @unit rad
 */
PARAM_DEFINE_FLOAT(EKF2_EVA_NOISE, 0.1);

/**
 * Gate size for vision position fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_EVP_GATE, 5.0);

/**
 * Measurement noise for vision position measurements
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_EVP_NOISE, 0.1);

/**
 * Gate size for vision velocity estimate fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_EVV_GATE, 3.0);

/**
 * Measurement noise for vision velocity measurements
 *
 * Used to lower bound or replace the uncertainty included in the message
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_EVV_NOISE, 0.1);

/**
 * X position of VI sensor focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_EV_POS_X, 0.0);

/**
 * Y position of VI sensor focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_EV_POS_Y, 0.0);

/**
 * Z position of VI sensor focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_EV_POS_Z, 0.0);

/**
 * GNSS sensor aiding
 *
 * Set bits in the following positions to enable: 0 : Longitude and latitude fusion 1 : Altitude fusion 2 : 3D velocity fusion 3 : Dual antenna heading fusion
 *
 * @group EKF2
 * @bit 0 Lon/lat
 * @bit 1 Altitude
 * @bit 2 3D velocity
 * @bit 3 Dual antenna heading
 * @min 0
 * @max 15
 * @min 0
 * @max 15
 */
PARAM_DEFINE_INT32(EKF2_GPS_CTRL, 7);

/**
 * Fusion reset mode
 *
 * Automatic: reset on fusion timeout if no other source of position is available. Dead-reckoning: reset on fusion timeout if no source of velocity is available.
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Dead-reckoning
 */
PARAM_DEFINE_INT32(EKF2_GPS_MODE, 0);

/**
 * GPS measurement delay relative to IMU measurement
 *
 * GPS measurement delay relative to IMU measurement if PPS time correction is not available/enabled (PPS_CAP_ENABLE).
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_DELAY, 110);

/**
 * Measurement noise for GNSS position
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @max 10.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_P_NOISE, 0.5);

/**
 * Gate size for GNSS position fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_P_GATE, 5.0);

/**
 * Heading/Yaw offset for dual antenna GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 360.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_YAW_OFF, 0.0);

/**
 * Gate size for GNSS velocity fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_V_GATE, 5.0);

/**
 * Measurement noise for GNSS velocity
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @max 5.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_V_NOISE, 0.3);

/**
 * X position of GPS antenna in body frame
 *
 * Forward (roll) axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_POS_X, 0.0);

/**
 * Y position of GPS antenna in body frame
 *
 * Right (pitch) axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_POS_Y, 0.0);

/**
 * Z position of GPS antenna in body frame
 *
 * Down (yaw) axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_GPS_POS_Z, 0.0);

/**
 * Integer bitmask controlling GPS checks
 *
 * Each threshold value is defined by the parameter indicated next to the check. Drift and offset checks only run when the vehicle is on ground and stationary.
 *
 * @group EKF2
 * @bit 0 Sat count (EKF2_REQ_NSATS)
 * @bit 1 PDOP (EKF2_REQ_PDOP)
 * @bit 2 EPH (EKF2_REQ_EPH)
 * @bit 3 EPV (EKF2_REQ_EPV)
 * @bit 4 Speed accuracy (EKF2_REQ_SACC)
 * @bit 5 Horizontal position drift (EKF2_REQ_HDRIFT)
 * @bit 6 Vertical position drift (EKF2_REQ_VDRIFT)
 * @bit 7 Horizontal speed offset (EKF2_REQ_HDRIFT)
 * @bit 8 Vertical speed offset (EKF2_REQ_VDRIFT)
 * @bit 9 Spoofing
 * @bit 10 GPS fix type (EKF2_REQ_FIX)
 * @bit 11 Jamming
 * @min 0
 * @max 4095
 * @min 0
 * @max 4095
 */
PARAM_DEFINE_INT32(EKF2_GPS_CHECK, 2047);

/**
 * Required EPH to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 2
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_EPH, 3.0);

/**
 * Required EPV to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 2
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_EPV, 5.0);

/**
 * Required speed accuracy to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.5
 * @max 5.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_SACC, 0.5);

/**
 * Required satellite count to use GPS
 *
 * 
 *
 * @group EKF2
 * @min 4
 * @max 12
 */
PARAM_DEFINE_INT32(EKF2_REQ_NSATS, 6);

/**
 * Maximum PDOP to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 1.5
 * @max 5.0
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_PDOP, 2.5);

/**
 * Maximum horizontal drift speed to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 1.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_HDRIFT, 0.1);

/**
 * Maximum vertical drift speed to use GPS
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 1.5
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_VDRIFT, 0.2);

/**
 * Required GPS fix
 *
 * Minimum GPS fix type required for GPS usage.
 *
 * @group EKF2
 * @value 0 No fix required
 * @value 2 2D fix
 * @value 3 3D fix
 * @value 4 RTCM code differential
 * @value 5 RTK float
 * @value 6 RTK fixed
 * @value 8 Extrapolated
 */
PARAM_DEFINE_INT32(EKF2_REQ_FIX, 3);

/**
 * Required GPS health time on startup
 *
 * Minimum continuous period without GPS failure required to mark a healthy GPS status. It can be reduced to speed up initialization, but it's recommended to keep this unchanged for a vehicle.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.1
 * @unit s
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_REQ_GPS_H, 10.0);

/**
 * Default value of true airspeed used in EKF-GSF AHRS calculation
 *
 * If no airspeed measurements are available, the EKF-GSF AHRS calculation will assume this value of true airspeed when compensating for centripetal acceleration during turns. Set to zero to disable centripetal acceleration compensation during fixed wing flight modes.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 100.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_GSF_TAS, 15.0);

/**
 * Accelerometer measurement noise for gravity based observations
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 10.0
 * @unit g0
 */
PARAM_DEFINE_FLOAT(EKF2_GRAV_NOISE, 1.0);

/**
 * 1-sigma IMU gyro switch-on bias
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @max 0.2
 * @unit rad/s
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_GBIAS_INIT, 0.1);

/**
 * Process noise for IMU rate gyro bias prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 6
 * @min 0.0
 * @max 0.01
 * @unit rad/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_GYR_B_NOISE, 0.001);

/**
 * Gyro bias learning limit
 *
 * The ekf gyro bias states will be limited to within a range equivalent to +- of this value.
 *
 * @group EKF2
 * @decimal 3
 * @min 0.0
 * @max 0.4
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(EKF2_GYR_B_LIM, 0.15);

/**
 * Type of magnetometer fusion
 *
 * Integer controlling the type of magnetometer fusion used - magnetic heading or 3-component vector. The fusion of magnetometer data as a three component vector enables vehicle body fixed hard iron errors to be learned, but requires a stable earth field. If set to 'Automatic' magnetic heading fusion is used when on-ground and 3-axis magnetic field fusion in-flight. If set to 'Magnetic heading' magnetic heading fusion is used at all times. If set to 'None' the magnetometer will not be used under any circumstance. If no external source of yaw is available, it is possible to use post-takeoff horizontal movement combined with GNSS velocity measurements to align the yaw angle. If set to 'Init' the magnetometer is only used to initalize the heading.
 *
 * @group EKF2
 * @value 0 Automatic
 * @value 1 Magnetic heading
 * @value 5 None
 * @value 6 Init
 * @reboot_required True
 */
PARAM_DEFINE_INT32(EKF2_MAG_TYPE, 0);

/**
 * Magnetometer measurement delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_DELAY, 0);

/**
 * Gate size for magnetometer XYZ component fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_GATE, 3.0);

/**
 * Measurement noise for magnetometer 3-axis fusion
 *
 * 
 *
 * @group EKF2
 * @decimal 3
 * @min 0.001
 * @max 1.0
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_NOISE, 0.05);

/**
 * Process noise for body magnetic field prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 6
 * @min 0.0
 * @max 0.1
 * @unit gauss/s
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_B_NOISE, 0.0001);

/**
 * Process noise for earth magnetic field prediction
 *
 * 
 *
 * @group EKF2
 * @decimal 6
 * @min 0.0
 * @max 0.1
 * @unit gauss/s
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_E_NOISE, 0.001);

/**
 * Integer bitmask controlling handling of magnetic declination
 *
 * Set bits in the following positions to enable functions. 0 : Set to true to use the declination from the geo_lookup library when the GPS position becomes available, set to false to always use the EKF2_MAG_DECL value. 1 : Set to true to save the EKF2_MAG_DECL parameter to the value returned by the EKF when the vehicle disarms.
 *
 * @group EKF2
 * @bit 0 use geo_lookup declination
 * @bit 1 save EKF2_MAG_DECL on disarm
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 * @reboot_required True
 */
PARAM_DEFINE_INT32(EKF2_DECL_TYPE, 3);

/**
 * Horizontal acceleration threshold used for heading observability check
 *
 * The heading is assumed to be observable when the body acceleration is greater than this parameter when a global position/velocity aiding source is active.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @max 5.0
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_ACCLIM, 0.5);

/**
 * Magnetic field strength test selection
 *
 * Bitmask to set which check is used to decide whether the magnetometer data is valid. If GNSS data is received, the magnetic field is compared to a World Magnetic Model (WMM), otherwise an average value is used. This check is useful to reject occasional hard iron disturbance. Set bits to 1 to enable checks. Checks enabled by the following bit positions 0 : Magnetic field strength. Set tolerance using EKF2_MAG_CHK_STR 1 : Magnetic field inclination. Set tolerance using EKF2_MAG_CHK_INC 2 : Wait for GNSS to find the theoretical strength and inclination using the WMM
 *
 * @group EKF2
 * @bit 0 Strength (EKF2_MAG_CHK_STR)
 * @bit 1 Inclination (EKF2_MAG_CHK_INC)
 * @bit 2 Wait for WMM
 * @min 0
 * @max 7
 * @min 0
 * @max 7
 */
PARAM_DEFINE_INT32(EKF2_MAG_CHECK, 1);

/**
 * Magnetic field strength check tolerance
 *
 * Maximum allowed deviation from the expected magnetic field strength to pass the check.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @max 1.0
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_CHK_STR, 0.2);

/**
 * Magnetic field inclination check tolerance
 *
 * Maximum allowed deviation from the expected magnetic field inclination to pass the check.
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 90.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_CHK_INC, 20.0);

/**
 * Enable synthetic magnetometer Z component measurement
 *
 * Use for vehicles where the measured body Z magnetic field is subject to strong magnetic interference. For magnetic heading fusion the magnetometer Z measurement will be replaced by a synthetic value calculated using the knowledge of the 3D magnetic field vector at the location of the drone. Therefore, this parameter will only have an effect if the global position of the drone is known. For 3D mag fusion the magnetometer Z measurement will simply be ignored instead of fusing the synthetic value.
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_SYNT_MAG_Z, 0);

/**
 * Multi-EKF IMUs
 *
 * Maximum number of IMUs to use for Multi-EKF. Set 0 to disable. Requires SENS_IMU_MODE 0.
 *
 * @group EKF2
 * @min 0
 * @max 4
 * @reboot_required True
 */
PARAM_DEFINE_INT32(EKF2_MULTI_IMU, 0);

/**
 * Multi-EKF Magnetometers
 *
 * Maximum number of magnetometers to use for Multi-EKF. Set 0 to disable. Requires SENS_MAG_MODE 0.
 *
 * @group EKF2
 * @min 0
 * @max 4
 * @reboot_required True
 */
PARAM_DEFINE_INT32(EKF2_MULTI_MAG, 0);

/**
 * Optical flow aiding
 *
 * Enable optical flow fusion.
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_OF_CTRL, 1);

/**
 * Optical flow measurement delay relative to IMU measurements
 *
 * Assumes measurement is timestamped at trailing edge of integration period
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_OF_DELAY, 20);

/**
 * Gate size for optical flow fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_OF_GATE, 3.0);

/**
 * Optical flow minimum noise
 *
 * Measurement noise for the optical flow sensor when it's reported quality metric is at the maximum
 *
 * @group EKF2
 * @decimal 2
 * @min 0.05
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(EKF2_OF_N_MIN, 0.15);

/**
 * Optical flow maximum noise
 *
 * Measurement noise for the optical flow sensor when it's reported quality metric is at the minimum
 *
 * @group EKF2
 * @decimal 2
 * @min 0.05
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(EKF2_OF_N_MAX, 0.5);

/**
 * In air optical flow minimum quality
 *
 * Optical Flow data will only be used in air if the sensor reports a quality metric >= EKF2_OF_QMIN
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_OF_QMIN, 1);

/**
 * On ground optical flow minimum quality
 *
 * Optical Flow data will only be used on the ground if the sensor reports a quality metric >= EKF2_OF_QMIN_GND
 *
 * @group EKF2
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(EKF2_OF_QMIN_GND, 0);

/**
 * Optical flow angular rate compensation source
 *
 * Auto: use gyro from optical flow message if available, internal gyro otherwise. Internal: always use internal gyro
 *
 * @group EKF2
 * @value 0 Auto
 * @value 1 Internal
 */
PARAM_DEFINE_INT32(EKF2_OF_GYR_SRC, 0);

/**
 * X position of optical flow focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_OF_POS_X, 0.0);

/**
 * Y position of optical flow focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_OF_POS_Y, 0.0);

/**
 * Z position of optical flow focal point in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_OF_POS_Z, 0.0);

/**
 * Range sensor height aiding
 *
 * WARNING: Range finder measurements are less reliable and can experience unexpected errors. For these reasons, if accurate control of height relative to ground is required, it is recommended to use the MPC_ALT_MODE parameter instead, unless baro errors are severe enough to cause problems with landing and takeoff. If this parameter is enabled then the estimator will make use of the range finder measurements to estimate its height in addition to other height sources (if activated). Range sensor aiding can be enabled (i.e.: always use) or set in "conditional" mode. Conditional mode: This enables the range finder to be used during low speed (< EKF2_RNG_A_VMAX) and low altitude (< EKF2_RNG_A_HMAX) operation, eg takeoff and landing, where baro interference from rotor wash is excessive and can corrupt EKF state estimates. It is intended to be used where a vertical takeoff and landing is performed, and horizontal flight does not occur until above EKF2_RNG_A_HMAX.
 *
 * @group EKF2
 * @value 0 Disable range fusion
 * @value 1 Enabled (conditional mode)
 * @value 2 Enabled
 */
PARAM_DEFINE_INT32(EKF2_RNG_CTRL, 1);

/**
 * Range finder measurement delay relative to IMU measurements
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0
 * @max 300
 * @unit ms
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_DELAY, 5);

/**
 * Gate size for range finder fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_GATE, 5.0);

/**
 * Measurement noise for range finder fusion
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_NOISE, 0.1);

/**
 * Range sensor pitch offset
 *
 * 
 *
 * @group EKF2
 * @decimal 3
 * @min -0.75
 * @max 0.75
 * @unit rad
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_PITCH, 0.0);

/**
 * Maximum horizontal velocity allowed for conditional range aid mode
 *
 * If the vehicle horizontal speed exceeds this value then the estimator will not fuse range measurements to estimate its height. This only applies when conditional range aid mode is activated (EKF2_RNG_CTRL = 1).
 *
 * @group EKF2
 * @min 0.1
 * @max 2
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_A_VMAX, 1.0);

/**
 * Maximum height above ground allowed for conditional range aid mode
 *
 * If the vehicle absolute altitude exceeds this value then the estimator will not fuse range measurements to estimate its height. This only applies when conditional range aid mode is activated (EKF2_RNG_CTRL = 1).
 *
 * @group EKF2
 * @min 1.0
 * @max 10.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_A_HMAX, 5.0);

/**
 * Minumum range validity period
 *
 * Minimum duration during which the reported range finder signal quality needs to be non-zero in order to be declared valid (s)
 *
 * @group EKF2
 * @min 0.1
 * @max 5
 * @unit s
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_QLTY_T, 1.0);

/**
 * Gate size used for range finder kinematic consistency check
 *
 * To be used, the time derivative of the distance sensor measurements projected on the vertical axis needs to be statistically consistent with the estimated vertical velocity of the drone. Decrease this value to make the filter more robust against range finder faulty data (stuck, reflections, ...). Note: tune the range finder noise parameters (EKF2_RNG_NOISE and EKF2_RNG_SFE) before tuning this gate.
 *
 * @group EKF2
 * @min 0.1
 * @max 5.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_K_GATE, 1.0);

/**
 * Range finder range dependent noise scaler
 *
 * Specifies the increase in range finder noise with range.
 *
 * @group EKF2
 * @min 0.0
 * @max 0.2
 * @unit m/m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_SFE, 0.05);

/**
 * X position of range finder origin in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_POS_X, 0.0);

/**
 * Y position of range finder origin in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_POS_Y, 0.0);

/**
 * Z position of range finder origin in body frame
 *
 * Forward axis with origin relative to vehicle centre of gravity
 *
 * @group EKF2
 * @decimal 3
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_POS_Z, 0.0);

/**
 * Maximum distance at which the range finder could detect fog (m)
 *
 * Limit for fog detection. If the range finder measures a distance greater than this value, the measurement is considered to not be blocked by fog or rain. If there's a jump from larger than RNG_FOG to smaller than EKF2_RNG_FOG, the measurement may gets rejected. 0 - disabled
 *
 * @group EKF2
 * @decimal 1
 * @min 0.0
 * @max 20.0
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_RNG_FOG, 3.0);

/**
 * Selector error reduce threshold
 *
 * EKF2 instances have to be better than the selected by at least this amount before their relative score can be reduced.
 *
 * @group EKF2
 */
PARAM_DEFINE_FLOAT(EKF2_SEL_ERR_RED, 0.2);

/**
 * Selector angular rate threshold
 *
 * EKF2 selector angular rate error threshold for comparing gyros. Angular rate vector differences larger than this will result in accumulated angular error.
 *
 * @group EKF2
 * @unit deg/s
 */
PARAM_DEFINE_FLOAT(EKF2_SEL_IMU_RAT, 7.0);

/**
 * Selector angular threshold
 *
 * EKF2 selector maximum accumulated angular error threshold for comparing gyros. Accumulated angular error larger than this will result in the sensor being declared faulty.
 *
 * @group EKF2
 * @unit deg
 */
PARAM_DEFINE_FLOAT(EKF2_SEL_IMU_ANG, 15.0);

/**
 * Selector acceleration threshold
 *
 * EKF2 selector acceleration error threshold for comparing accelerometers. Acceleration vector differences larger than this will result in accumulated velocity error.
 *
 * @group EKF2
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(EKF2_SEL_IMU_ACC, 1.0);

/**
 * Selector angular threshold
 *
 * EKF2 selector maximum accumulated velocity threshold for comparing accelerometers. Accumulated velocity error larger than this will result in the sensor being declared faulty.
 *
 * @group EKF2
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_SEL_IMU_VEL, 2.0);

/**
 * Enable synthetic sideslip fusion
 *
 * For reliable wind estimation both sideslip and airspeed fusion (see EKF2_ARSP_THR) should be enabled. Only applies to vehicles in fixed-wing mode or with airspeed fusion active. Note: side slip fusion is currently not supported for tailsitters.
 *
 * @group EKF2
 * @boolean
 */
PARAM_DEFINE_INT32(EKF2_FUSE_BETA, 0);

/**
 * Gate size for synthetic sideslip fusion
 *
 * Sets the number of standard deviations used by the innovation consistency test.
 *
 * @group EKF2
 * @decimal 1
 * @min 1.0
 * @unit SD
 */
PARAM_DEFINE_FLOAT(EKF2_BETA_GATE, 5.0);

/**
 * Noise for synthetic sideslip fusion
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.1
 * @max 1.0
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_BETA_NOISE, 0.3);

/**
 * Terrain altitude process noise
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @min 0.5
 * @unit m/s
 */
PARAM_DEFINE_FLOAT(EKF2_TERR_NOISE, 5.0);

/**
 * Magnitude of terrain gradient
 *
 * 
 *
 * @group EKF2
 * @decimal 2
 * @min 0.0
 * @unit m/m
 */
PARAM_DEFINE_FLOAT(EKF2_TERR_GRAD, 0.5);

/**
 * Expected range finder reading when on ground
 *
 * If the vehicle is on ground, is not moving as determined by the motion test and the range finder is returning invalid or no data, then an assumed range value of EKF2_MIN_RNG will be used by the terrain estimator so that a terrain height estimate is available at the start of flight in situations where the range finder may be inside its minimum measurements distance when on ground.
 *
 * @group EKF2
 * @decimal 2
 * @min 0.01
 * @unit m
 */
PARAM_DEFINE_FLOAT(EKF2_MIN_RNG, 0.01);

/**
 * Magnetic declination
 *
 * 
 *
 * @group EKF2
 * @decimal 1
 * @category System
 * @volatile True
 * @unit deg
 */
PARAM_DEFINE_FLOAT(EKF2_MAG_DECL, 0);

/**
 * Process noise spectral density for wind velocity prediction
 *
 * When unaided, the wind estimate uncertainty (1-sigma, in m/s) increases by this amount every second.
 *
 * @group EKF2
 * @decimal 3
 * @min 0.0
 * @max 1.0
 * @unit m/s^2/sqrt(Hz)
 */
PARAM_DEFINE_FLOAT(EKF2_WIND_NSD, 0.05);

/**
 * Enable internal combustion engine
 *
 * 
 *
 * @group ICE
 * @boolean
 */
PARAM_DEFINE_INT32(ICE_EN, 0);

/**
 * Engine start/stop input source
 *
 * 
 *
 * @group ICE
 * @value 0 On arming - disarming
 * @value 1 Aux1
 * @value 2 Aux2
 * @value 3 On Vtol Transitions
 */
PARAM_DEFINE_INT32(ICE_ON_SOURCE, 0);

/**
 * Duration of choking during startup
 *
 * 
 *
 * @group ICE
 * @decimal 1
 * @increment 0.1
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(ICE_CHOKE_ST_DUR, 5);

/**
 * Duration of single starting attempt (excl. choking)
 *
 * Maximum expected time for startup before declaring timeout.
 * 
 *
 * @group ICE
 * @decimal 1
 * @increment 0.1
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(ICE_STRT_DUR, 5);

/**
 * Minimum RPM for engine to be declared running
 *
 * 
 *
 * @group ICE
 * @decimal 0
 * @increment 1
 * @min 0
 * @max 10000
 * @unit rpm
 */
PARAM_DEFINE_FLOAT(ICE_MIN_RUN_RPM, 2000);

/**
 * Number attempts for starting the engine
 *
 * Number of accepted attempts for starting the engine before declaring a fault.
 * 
 *
 * @group ICE
 * @min 0
 * @max 10
 */
PARAM_DEFINE_INT32(ICE_STRT_ATTEMPT, 3);

/**
 * Fault detection if it stops in running state
 *
 * Enables restart if a fault is detected during the running state. Otherwise
 * commands continues in running state until given an user request off.
 * 
 *
 * @group ICE
 * @boolean
 */
PARAM_DEFINE_INT32(ICE_RUN_FAULT_D, 1);

/**
 * Throttle value for starting engine
 *
 * During the choking and the starting phase, the throttle value is set to this value.
 * 
 *
 * @group ICE
 * @decimal 0
 * @increment 0.01
 * @min 0
 * @max 1
 * @unit norm
 */
PARAM_DEFINE_FLOAT(ICE_STRT_THR, 0.1);

/**
 * Apply choke when stopping engine
 *
 * 
 *
 * @group ICE
 * @boolean
 */
PARAM_DEFINE_INT32(ICE_STOP_CHOKE, 1);

/**
 * Throttle slew rate
 *
 * Maximum rate of change of throttle value per second.
 * 
 *
 * @group ICE
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 1
 * @unit 1/s
 */
PARAM_DEFINE_FLOAT(ICE_THR_SLEW, 0.5);

/**
 * Cold-start delay after ignition before engaging starter
 *
 * In case that the ignition takes a moment to be up and running, this parameter can be set to account for that.
 * 
 *
 * @group ICE
 * @decimal 1
 * @increment 0.1
 * @min 0
 * @max 10
 * @unit s
 */
PARAM_DEFINE_FLOAT(ICE_IGN_DELAY, 0);

/**
 * UTC offset (unit: min)
 *
 * the difference in hours and minutes from Coordinated Universal Time (UTC) for a your place and date. for example, In case of South Korea(UTC+09:00), UTC offset is 540 min (9*60) refer to https://en.wikipedia.org/wiki/List_of_UTC_time_offsets
 *
 * @group SD Logging
 * @min -1000
 * @max 1000
 * @unit min
 */
PARAM_DEFINE_INT32(SDLOG_UTC_OFFSET, 0);

/**
 * Logging Mode
 *
 * Determines when to start and stop logging. By default, logging is started when arming the system, and stopped when disarming. Note: The logging start/end points that can be configured here only apply to SD logging. The mavlink backend is started/stopped independently of these points.
 *
 * @group SD Logging
 * @value 0 when armed until disarm (default)
 * @value 1 from boot until disarm
 * @value 2 from boot until shutdown
 * @value 3 while manual input AUX1 >30%
 * @value 4 from 1st armed until shutdown
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SDLOG_MODE, 0);

/**
 * Logging Backend (integer bitmask)
 *
 * If no logging is set the logger will not be started. Set bits true to enable: 0: SD card logging 1: Mavlink logging
 *
 * @group SD Logging
 * @bit 0 SD card logging
 * @bit 1 Mavlink logging
 * @min 0
 * @max 3
 * @min 0
 * @max 3
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SDLOG_BACKEND, 3);

/**
 * Battery-only Logging
 *
 * When enabled, logging will not start from boot if battery power is not detected (e.g. powered via USB on a test bench). This prevents extraneous flight logs from being created during bench testing. Note that this only applies to log-from-boot modes. This has no effect on arm-based modes.
 *
 * @group SD Logging
 * @boolean
 */
PARAM_DEFINE_INT32(SDLOG_BOOT_BAT, 0);

/**
 * Mission Log
 *
 * If enabled, a small additional "mission" log file will be written to the SD card. The log contains just those messages that are useful for tasks like generating flight statistics and geotagging. The different modes can be used to further reduce the logged data (and thus the log file size). For example, choose geotagging mode to only log data required for geotagging. Note that the normal/full log is still created, and contains all the data in the mission log (and more).
 *
 * @group SD Logging
 * @value 0 Disabled
 * @value 1 All mission messages
 * @value 2 Geotagging messages
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SDLOG_MISSION, 0);

/**
 * Logging topic profile (integer bitmask)
 *
 * This integer bitmask controls the set and rates of logged topics. The default allows for general log analysis while keeping the log file size reasonably small. Enabling multiple sets leads to higher bandwidth requirements and larger log files. Set bits true to enable: 0 : Default set (used for general log analysis) 1 : Full rate estimator (EKF2) replay topics 2 : Topics for thermal calibration (high rate raw IMU and Baro sensor data) 3 : Topics for system identification (high rate actuator control and IMU data) 4 : Full rates for analysis of fast maneuvers (RC, attitude, rates and actuators) 5 : Debugging topics (debug_*.msg topics, for custom code) 6 : Topics for sensor comparison (low rate raw IMU, Baro and magnetometer data) 7 : Topics for computer vision and collision prevention 8 : Raw FIFO high-rate IMU (Gyro) 9 : Raw FIFO high-rate IMU (Accel) 10: Logging of mavlink tunnel message (useful for payload communication debugging)
 *
 * @group SD Logging
 * @bit 0 Default set (general log analysis)
 * @bit 1 Estimator replay (EKF2)
 * @bit 2 Thermal calibration
 * @bit 3 System identification
 * @bit 4 High rate
 * @bit 5 Debug
 * @bit 6 Sensor comparison
 * @bit 7 Computer Vision and Avoidance
 * @bit 8 Raw FIFO high-rate IMU (Gyro)
 * @bit 9 Raw FIFO high-rate IMU (Accel)
 * @bit 10 Mavlink tunnel message logging
 * @bit 11 High rate sensors
 * @min 0
 * @max 4095
 * @min 0
 * @max 4095
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SDLOG_PROFILE, 1);

/**
 * Maximum number of log directories to keep
 *
 * If there are more log directories than this value, the system will delete the oldest directories during startup. In addition, the system will delete old logs if there is not enough free space left. The minimum amount is 300 MB. If this is set to 0, old directories will only be removed if the free space falls below the minimum. Note: this does not apply to mission log files.
 *
 * @group SD Logging
 * @min 0
 * @max 1000
 * @reboot_required True
 */
PARAM_DEFINE_INT32(SDLOG_DIRS_MAX, 0);

/**
 * Log UUID
 *
 * If set to 1, add an ID to the log, which uniquely identifies the vehicle
 *
 * @group SD Logging
 * @boolean
 */
PARAM_DEFINE_INT32(SDLOG_UUID, 1);

/**
 * Logfile Encryption algorithm
 *
 * Selects the algorithm used for logfile encryption
 *
 * @group SD Logging
 * @value 0 Disabled
 * @value 2 XChaCha20
 */
PARAM_DEFINE_INT32(SDLOG_ALGORITHM, 2);

/**
 * Logfile Encryption key index
 *
 * Selects the key in keystore, used for encrypting the log. When using a symmetric encryption algorithm, the key is generated at logging start and kept stored in this index. For symmetric algorithms, the key is volatile and valid only for the duration of logging. The key is stored in encrypted format on the sdcard alongside the logfile, using an RSA2048 key defined by the SDLOG_EXCHANGE_KEY
 *
 * @group SD Logging
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(SDLOG_KEY, 2);

/**
 * Logfile Encryption key exchange key
 *
 * If the logfile is encrypted using a symmetric key algorithm, the used encryption key is generated at logging start and stored on the sdcard RSA2048 encrypted using this key.
 *
 * @group SD Logging
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(SDLOG_EXCH_KEY, 1);

/**
 * MAVLink Mode for instance 0
 *
 * The MAVLink Mode defines the set of streamed messages (for example the
 * vehicle's attitude) and their sending rates.
 * 
 *
 * @group MAVLink
 * @value 0 Normal
 * @value 1 Custom
 * @value 2 Onboard
 * @value 3 OSD
 * @value 4 Magic
 * @value 5 Config
 * @value 7 Minimal
 * @value 8 External Vision
 * @value 10 Gimbal
 * @value 11 Onboard Low Bandwidth
 * @value 12 uAvionix
 * @value 13 Low Bandwidth
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_0_MODE, 0);

/**
 * MAVLink Mode for instance 1
 *
 * The MAVLink Mode defines the set of streamed messages (for example the
 * vehicle's attitude) and their sending rates.
 * 
 *
 * @group MAVLink
 * @value 0 Normal
 * @value 1 Custom
 * @value 2 Onboard
 * @value 3 OSD
 * @value 4 Magic
 * @value 5 Config
 * @value 7 Minimal
 * @value 8 External Vision
 * @value 10 Gimbal
 * @value 11 Onboard Low Bandwidth
 * @value 12 uAvionix
 * @value 13 Low Bandwidth
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_1_MODE, 2);

/**
 * MAVLink Mode for instance 2
 *
 * The MAVLink Mode defines the set of streamed messages (for example the
 * vehicle's attitude) and their sending rates.
 * 
 *
 * @group MAVLink
 * @value 0 Normal
 * @value 1 Custom
 * @value 2 Onboard
 * @value 3 OSD
 * @value 4 Magic
 * @value 5 Config
 * @value 7 Minimal
 * @value 8 External Vision
 * @value 10 Gimbal
 * @value 11 Onboard Low Bandwidth
 * @value 12 uAvionix
 * @value 13 Low Bandwidth
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_2_MODE, 0);

/**
 * Maximum MAVLink sending rate for instance 0
 *
 * Configure the maximum sending rate for the MAVLink streams in Bytes/sec.
 * If the configured streams exceed the maximum rate, the sending rate of
 * each stream is automatically decreased.
 * 
 * If this is set to 0 a value of half of the theoretical maximum bandwidth is used.
 * This corresponds to baudrate/20 Bytes/s (baudrate/10 = maximum data rate on
 * 8N1-configured links).
 * 
 *
 * @group MAVLink
 * @min 0
 * @unit B/s
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_0_RATE, 1200);

/**
 * Maximum MAVLink sending rate for instance 1
 *
 * Configure the maximum sending rate for the MAVLink streams in Bytes/sec.
 * If the configured streams exceed the maximum rate, the sending rate of
 * each stream is automatically decreased.
 * 
 * If this is set to 0 a value of half of the theoretical maximum bandwidth is used.
 * This corresponds to baudrate/20 Bytes/s (baudrate/10 = maximum data rate on
 * 8N1-configured links).
 * 
 *
 * @group MAVLink
 * @min 0
 * @unit B/s
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_1_RATE, 0);

/**
 * Maximum MAVLink sending rate for instance 2
 *
 * Configure the maximum sending rate for the MAVLink streams in Bytes/sec.
 * If the configured streams exceed the maximum rate, the sending rate of
 * each stream is automatically decreased.
 * 
 * If this is set to 0 a value of half of the theoretical maximum bandwidth is used.
 * This corresponds to baudrate/20 Bytes/s (baudrate/10 = maximum data rate on
 * 8N1-configured links).
 * 
 *
 * @group MAVLink
 * @min 0
 * @unit B/s
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_2_RATE, 0);

/**
 * Enable MAVLink Message forwarding for instance 0
 *
 * If enabled, forward incoming MAVLink messages to other MAVLink ports if the
 * message is either broadcast or the target is not the autopilot.
 * 
 * This allows for example a GCS to talk to a camera that is connected to the
 * autopilot via MAVLink (on a different link than the GCS).
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_0_FORWARD, 1);

/**
 * Enable MAVLink Message forwarding for instance 1
 *
 * If enabled, forward incoming MAVLink messages to other MAVLink ports if the
 * message is either broadcast or the target is not the autopilot.
 * 
 * This allows for example a GCS to talk to a camera that is connected to the
 * autopilot via MAVLink (on a different link than the GCS).
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_1_FORWARD, 0);

/**
 * Enable MAVLink Message forwarding for instance 2
 *
 * If enabled, forward incoming MAVLink messages to other MAVLink ports if the
 * message is either broadcast or the target is not the autopilot.
 * 
 * This allows for example a GCS to talk to a camera that is connected to the
 * autopilot via MAVLink (on a different link than the GCS).
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_2_FORWARD, 0);

/**
 * Enable software throttling of mavlink on instance 0
 *
 * If enabled, MAVLink messages will be throttled according to
 * `txbuf` field reported by radio_status.
 * 
 * Requires a radio to send the mavlink message RADIO_STATUS.
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_0_RADIO_CTL, 1);

/**
 * Enable software throttling of mavlink on instance 1
 *
 * If enabled, MAVLink messages will be throttled according to
 * `txbuf` field reported by radio_status.
 * 
 * Requires a radio to send the mavlink message RADIO_STATUS.
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_1_RADIO_CTL, 1);

/**
 * Enable software throttling of mavlink on instance 2
 *
 * If enabled, MAVLink messages will be throttled according to
 * `txbuf` field reported by radio_status.
 * 
 * Requires a radio to send the mavlink message RADIO_STATUS.
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_2_RADIO_CTL, 1);

/**
 * Enable serial flow control for instance 0
 *
 * This is used to force flow control on or off for the the mavlink
 * instance. By default it is auto detected. Use when auto detection fails.
 * 
 *
 * @group MAVLink
 * @value 0 Force off
 * @value 1 Force on
 * @value 2 Auto-detected
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_0_FLOW_CTRL, 2);

/**
 * Enable serial flow control for instance 1
 *
 * This is used to force flow control on or off for the the mavlink
 * instance. By default it is auto detected. Use when auto detection fails.
 * 
 *
 * @group MAVLink
 * @value 0 Force off
 * @value 1 Force on
 * @value 2 Auto-detected
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_1_FLOW_CTRL, 2);

/**
 * Enable serial flow control for instance 2
 *
 * This is used to force flow control on or off for the the mavlink
 * instance. By default it is auto detected. Use when auto detection fails.
 * 
 *
 * @group MAVLink
 * @value 0 Force off
 * @value 1 Force on
 * @value 2 Auto-detected
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_2_FLOW_CTRL, 2);

/**
 * Configures the frequency of HIGH_LATENCY2 stream for instance 0
 *
 * Positive real value that configures the transmission frequency of the
 * HIGH_LATENCY2 stream for instance 0, configured in iridium mode.
 * This parameter has no effect if the instance mode is different from iridium.
 * 
 *
 * @group MAVLink
 * @decimal 3
 * @increment 0.001
 * @min 0.0
 * @max 50.0
 * @unit Hz
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MAV_0_HL_FREQ, 0.015);

/**
 * Configures the frequency of HIGH_LATENCY2 stream for instance 1
 *
 * Positive real value that configures the transmission frequency of the
 * HIGH_LATENCY2 stream for instance 1, configured in iridium mode.
 * This parameter has no effect if the instance mode is different from iridium.
 * 
 *
 * @group MAVLink
 * @decimal 3
 * @increment 0.001
 * @min 0.0
 * @max 50.0
 * @unit Hz
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MAV_1_HL_FREQ, 0.015);

/**
 * Configures the frequency of HIGH_LATENCY2 stream for instance 2
 *
 * Positive real value that configures the transmission frequency of the
 * HIGH_LATENCY2 stream for instance 2, configured in iridium mode.
 * This parameter has no effect if the instance mode is different from iridium.
 * 
 *
 * @group MAVLink
 * @decimal 3
 * @increment 0.001
 * @min 0.0
 * @max 50.0
 * @unit Hz
 * @reboot_required True
 */
PARAM_DEFINE_FLOAT(MAV_2_HL_FREQ, 0.015);

/**
 * MAVLink Mode for SOM to FMU communication channel
 *
 * The MAVLink Mode defines the set of streamed messages (for example the
 * vehicle's attitude) and their sending rates.
 * 
 *
 * @group MAVLink
 * @value 0 Normal
 * @value 2 Onboard
 * @value 5 Config
 * @value 7 Minimal
 * @value 11 Onboard Low Bandwidth
 * @value 13 Low Bandwidth
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_S_MODE, 11);

/**
 * Enable MAVLink forwarding on TELEM2
 *
 * TELEM2 on Skynode only.
 * 
 *
 * @group MAVLink
 * @boolean
 * @reboot_required True
 */
PARAM_DEFINE_INT32(MAV_S_FORWARD, 0);

/**
 * Enable Raptor flight mode
 *
 * When enabled, the Raptor flight mode will be available. Please set MC_RAPTOR_OFFB according to your use case.
 * 
 *
 * @group Multicopter Raptor
 * @boolean
 * @category System
 */
PARAM_DEFINE_INT32(MC_RAPTOR_ENABLE, 0);

/**
 * Enable verbose output
 *
 * When enabled, the Raptor flight mode will print verbose output to the console.
 * 
 *
 * @group Multicopter Raptor
 * @boolean
 * @category System
 */
PARAM_DEFINE_INT32(MC_RAPTOR_VERBOS, 0);

/**
 * Enable Offboard mode replacement
 *
 * When enabled, the Raptor mode will replace the Offboard mode.
 * If disabled, the Raptor mode will be available as a separate external mode. In the latter case, Raptor will just hold the position, without requiring external setpoints. When Raptor replaces the Offboard mode, it requires external setpoints to be activated.
 * 
 *
 * @group Multicopter Raptor
 * @boolean
 * @category System
 */
PARAM_DEFINE_INT32(MC_RAPTOR_OFFB, 0);

/**
 * Use internal reference instead of trajectory_setpoint
 *
 * When enabled, instead of using the trajectory_setpoint, the position and yaw of the vehicle at the point when the Raptor mode is activated will be used as reference.
 * Use `mc_raptor intref lissajous <A> <B> <C> <fa> <fb> <fc> <duration> <ramp>` to configure the trajectory.
 * 
 *
 * @group Multicopter Raptor
 * @value 0 None
 * @value 1 Lissajous
 * @category System
 */
PARAM_DEFINE_INT32(MC_RAPTOR_INTREF, 0);

/**
 * Type of Gripper (Servo, etc.)
 *
 * 
 *
 * @group Payload Deliverer
 * @value -1 Undefined
 * @value 0 Servo
 * @min -1
 * @max 0
 */
PARAM_DEFINE_INT32(PD_GRIPPER_TYPE, 0);

/**
 * Timeout for successful gripper actuation acknowledgement
 *
 * Maximum time Gripper will wait while the successful griper actuation isn't recognised.
 * If the gripper has no feedback sensor, it will simply wait for
 * this time before considering gripper actuation successful and publish a
 * 'VehicleCommandAck' signaling successful gripper action
 * 
 *
 * @group Payload Deliverer
 * @min 0
 * @unit s
 */
PARAM_DEFINE_FLOAT(PD_GRIPPER_TO, 1);

/**
 * Wheel base
 *
 * Distance from the front to the rear axle.
 *
 * @group Rover Ackermann
 * @decimal 3
 * @increment 0.001
 * @min 0
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(RA_WHEEL_BASE, 0);

/**
 * Maximum steering angle
 *
 * 
 *
 * @group Rover Ackermann
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 1.5708
 * @unit rad
 */
PARAM_DEFINE_FLOAT(RA_MAX_STR_ANG, 0);

/**
 * Steering rate limit
 *
 * Set to -1 to disable.
 *
 * @group Rover Ackermann
 * @decimal 2
 * @increment 0.01
 * @min -1
 * @max 1000
 * @unit deg/s
 */
PARAM_DEFINE_FLOAT(RA_STR_RATE_LIM, -1);

/**
 * Maximum acceptance radius for the waypoints
 *
 * The controller scales the acceptance radius based on the angle between
 * the previous, current and next waypoint.
 * Higher value -> smoother trajectory at the cost of how close the rover gets
 * to the waypoint (Set to -1 to disable corner cutting).
 * 
 *
 * @group Rover Ackermann
 * @decimal 2
 * @increment 0.01
 * @min -1
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(RA_ACC_RAD_MAX, -1);

/**
 * Tuning parameter for corner cutting
 *
 * The geometric ideal acceptance radius is multiplied by this factor
 * to account for kinematic and dynamic effects.
 * Higher value -> The rover starts to cut the corner earlier.
 * 
 *
 * @group Rover Ackermann
 * @decimal 2
 * @increment 0.01
 * @min 1
 * @max 100
 */
PARAM_DEFINE_FLOAT(RA_ACC_RAD_GAIN, 1);

/**
 * Wheel track
 *
 * Distance from the center of the right wheel to the center of the left wheel.
 *
 * @group Rover Differential
 * @decimal 3
 * @increment 0.001
 * @min 0
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(RD_WHEEL_TRACK, 0);

/**
 * Yaw error threshhold to switch from spot turning to driving
 *
 * This threshold is used for the state machine to switch from turning to driving based on the
 * error between the desired and actual yaw.
 * 
 *
 * @group Rover Differential
 * @decimal 3
 * @increment 0.01
 * @min 0.001
 * @max 3.14159
 * @unit rad
 */
PARAM_DEFINE_FLOAT(RD_TRANS_TRN_DRV, 0.0872665);

/**
 * Yaw error threshhold to switch from driving to spot turning
 *
 * This threshold is used for the state machine to switch from driving to turning based on the
 * error between the desired and actual yaw. It is also used as the threshold whether the rover should come
 * to a smooth stop at the next waypoint. This slow down effect is active if the angle between the
 * line segments from prevWP-currWP and currWP-nextWP is smaller then 180 - RD_TRANS_DRV_TRN.
 * 
 *
 * @group Rover Differential
 * @decimal 3
 * @increment 0.01
 * @min 0.001
 * @max 3.14159
 * @unit rad
 */
PARAM_DEFINE_FLOAT(RD_TRANS_DRV_TRN, 0.174533);

/**
 * Yaw stick gain for Manual mode
 *
 * Assign value <1.0 to decrease stick response for yaw control.
 *
 * @group Rover Differential
 * @decimal 3
 * @increment 0.01
 * @min 0.1
 * @max 1
 */
PARAM_DEFINE_FLOAT(RD_YAW_STK_GAIN, 1);

/**
 * Wheel track
 *
 * Distance from the center of the right wheel to the center of the left wheel.
 *
 * @group Rover Mecanum
 * @decimal 3
 * @increment 0.001
 * @min 0
 * @max 100
 * @unit m
 */
PARAM_DEFINE_FLOAT(RM_WHEEL_TRACK, 0);

/**
 * Threshold to update course control in manual position mode
 *
 * Threshold for the angle between the active cruise direction and the cruise direction given
 * by the stick inputs.
 * This can be understood as a deadzone for the combined stick inputs for forward/backwards
 * and lateral speed which defines a course direction.
 * 
 *
 * @group Rover Mecanum
 * @decimal 2
 * @increment 0.01
 * @min 0
 * @max 3.14
 * @unit rad
 */
PARAM_DEFINE_FLOAT(RM_COURSE_CTL_TH, 0.17);

/**
 * Yaw stick gain for Manual mode
 *
 * Assign value <1.0 to decrease stick response for yaw control.
 *
 * @group Rover Mecanum
 * @decimal 3
 * @increment 0.01
 * @min 0.1
 * @max 1
 */
PARAM_DEFINE_FLOAT(RM_YAW_STK_GAIN, 1);

/**
 * Accelerometer 0 calibration device ID
 *
 * Device ID of the accelerometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC0_ID, 0);

/**
 * Accelerometer 1 calibration device ID
 *
 * Device ID of the accelerometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC1_ID, 0);

/**
 * Accelerometer 2 calibration device ID
 *
 * Device ID of the accelerometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC2_ID, 0);

/**
 * Accelerometer 3 calibration device ID
 *
 * Device ID of the accelerometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC3_ID, 0);

/**
 * Accelerometer 0 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC0_PRIO, -1);

/**
 * Accelerometer 1 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC1_PRIO, -1);

/**
 * Accelerometer 2 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC2_PRIO, -1);

/**
 * Accelerometer 3 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @decimal 3
 * @category System
 */
PARAM_DEFINE_INT32(CAL_ACC3_PRIO, -1);

/**
 * Accelerometer 0 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_ACC0_ROT, -1);

/**
 * Accelerometer 1 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_ACC1_ROT, -1);

/**
 * Accelerometer 2 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_ACC2_ROT, -1);

/**
 * Accelerometer 3 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_ACC3_ROT, -1);

/**
 * Accelerometer 0 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_XOFF, 0.0);

/**
 * Accelerometer 1 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_XOFF, 0.0);

/**
 * Accelerometer 2 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_XOFF, 0.0);

/**
 * Accelerometer 3 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_XOFF, 0.0);

/**
 * Accelerometer 0 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_YOFF, 0.0);

/**
 * Accelerometer 1 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_YOFF, 0.0);

/**
 * Accelerometer 2 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_YOFF, 0.0);

/**
 * Accelerometer 3 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_YOFF, 0.0);

/**
 * Accelerometer 0 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_ZOFF, 0.0);

/**
 * Accelerometer 1 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_ZOFF, 0.0);

/**
 * Accelerometer 2 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_ZOFF, 0.0);

/**
 * Accelerometer 3 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit m/s^2
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_ZOFF, 0.0);

/**
 * Accelerometer 0 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_XSCALE, 1.0);

/**
 * Accelerometer 1 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_XSCALE, 1.0);

/**
 * Accelerometer 2 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_XSCALE, 1.0);

/**
 * Accelerometer 3 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_XSCALE, 1.0);

/**
 * Accelerometer 0 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_YSCALE, 1.0);

/**
 * Accelerometer 1 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_YSCALE, 1.0);

/**
 * Accelerometer 2 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_YSCALE, 1.0);

/**
 * Accelerometer 3 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_YSCALE, 1.0);

/**
 * Accelerometer 0 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC0_ZSCALE, 1.0);

/**
 * Accelerometer 1 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC1_ZSCALE, 1.0);

/**
 * Accelerometer 2 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC2_ZSCALE, 1.0);

/**
 * Accelerometer 3 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_ACC3_ZSCALE, 1.0);

/**
 * Barometer 0 calibration device ID
 *
 * Device ID of the barometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO0_ID, 0);

/**
 * Barometer 1 calibration device ID
 *
 * Device ID of the barometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO1_ID, 0);

/**
 * Barometer 2 calibration device ID
 *
 * Device ID of the barometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO2_ID, 0);

/**
 * Barometer 3 calibration device ID
 *
 * Device ID of the barometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO3_ID, 0);

/**
 * Barometer 0 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO0_PRIO, -1);

/**
 * Barometer 1 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO1_PRIO, -1);

/**
 * Barometer 2 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO2_PRIO, -1);

/**
 * Barometer 3 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_BARO3_PRIO, -1);

/**
 * Barometer 0 offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_BARO0_OFF, 0.0);

/**
 * Barometer 1 offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_BARO1_OFF, 0.0);

/**
 * Barometer 2 offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_BARO2_OFF, 0.0);

/**
 * Barometer 3 offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_BARO3_OFF, 0.0);

/**
 * Gyroscope 0 calibration device ID
 *
 * Device ID of the gyroscope this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO0_ID, 0);

/**
 * Gyroscope 1 calibration device ID
 *
 * Device ID of the gyroscope this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO1_ID, 0);

/**
 * Gyroscope 2 calibration device ID
 *
 * Device ID of the gyroscope this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO2_ID, 0);

/**
 * Gyroscope 3 calibration device ID
 *
 * Device ID of the gyroscope this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO3_ID, 0);

/**
 * Gyroscope 0 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO0_PRIO, -1);

/**
 * Gyroscope 1 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO1_PRIO, -1);

/**
 * Gyroscope 2 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO2_PRIO, -1);

/**
 * Gyroscope 3 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_GYRO3_PRIO, -1);

/**
 * Gyroscope 0 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_GYRO0_ROT, -1);

/**
 * Gyroscope 1 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_GYRO1_ROT, -1);

/**
 * Gyroscope 2 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_GYRO2_ROT, -1);

/**
 * Gyroscope 3 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @category System
 * @min -1
 * @max 40
 */
PARAM_DEFINE_INT32(CAL_GYRO3_ROT, -1);

/**
 * Gyroscope 0 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO0_XOFF, 0.0);

/**
 * Gyroscope 1 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO1_XOFF, 0.0);

/**
 * Gyroscope 2 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO2_XOFF, 0.0);

/**
 * Gyroscope 3 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO3_XOFF, 0.0);

/**
 * Gyroscope 0 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO0_YOFF, 0.0);

/**
 * Gyroscope 1 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO1_YOFF, 0.0);

/**
 * Gyroscope 2 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO2_YOFF, 0.0);

/**
 * Gyroscope 3 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO3_YOFF, 0.0);

/**
 * Gyroscope 0 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO0_ZOFF, 0.0);

/**
 * Gyroscope 1 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO1_ZOFF, 0.0);

/**
 * Gyroscope 2 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO2_ZOFF, 0.0);

/**
 * Gyroscope 3 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit rad/s
 */
PARAM_DEFINE_FLOAT(CAL_GYRO3_ZOFF, 0.0);

/**
 * Magnetometer 0 calibration device ID
 *
 * Device ID of the magnetometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG0_ID, 0);

/**
 * Magnetometer 1 calibration device ID
 *
 * Device ID of the magnetometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG1_ID, 0);

/**
 * Magnetometer 2 calibration device ID
 *
 * Device ID of the magnetometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG2_ID, 0);

/**
 * Magnetometer 3 calibration device ID
 *
 * Device ID of the magnetometer this calibration applies to.
 *
 * @group Sensor Calibration
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG3_ID, 0);

/**
 * Magnetometer 0 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG0_PRIO, -1);

/**
 * Magnetometer 1 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG1_PRIO, -1);

/**
 * Magnetometer 2 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG2_PRIO, -1);

/**
 * Magnetometer 3 priority
 *
 * 
 *
 * @group Sensor Calibration
 * @value -1 Uninitialized
 * @value 0 Disabled
 * @value 1 Min
 * @value 25 Low
 * @value 50 Medium (Default)
 * @value 75 High
 * @value 100 Max
 * @category System
 */
PARAM_DEFINE_INT32(CAL_MAG3_PRIO, -1);

/**
 * Magnetometer 0 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * Set to "Custom Euler Angle" to define the rotation using CAL_MAG0_ROLL, CAL_MAG0_PITCH and CAL_MAG0_YAW.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @value 100 Custom Euler Angle
 * @category System
 * @min -1
 * @max 100
 */
PARAM_DEFINE_INT32(CAL_MAG0_ROT, -1);

/**
 * Magnetometer 1 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * Set to "Custom Euler Angle" to define the rotation using CAL_MAG1_ROLL, CAL_MAG1_PITCH and CAL_MAG1_YAW.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @value 100 Custom Euler Angle
 * @category System
 * @min -1
 * @max 100
 */
PARAM_DEFINE_INT32(CAL_MAG1_ROT, -1);

/**
 * Magnetometer 2 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * Set to "Custom Euler Angle" to define the rotation using CAL_MAG2_ROLL, CAL_MAG2_PITCH and CAL_MAG2_YAW.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @value 100 Custom Euler Angle
 * @category System
 * @min -1
 * @max 100
 */
PARAM_DEFINE_INT32(CAL_MAG2_ROT, -1);

/**
 * Magnetometer 3 rotation relative to airframe
 *
 * An internal sensor will force a value of -1, so a GCS should only attempt to configure the rotation if the value is greater than or equal to zero.
 * Set to "Custom Euler Angle" to define the rotation using CAL_MAG3_ROLL, CAL_MAG3_PITCH and CAL_MAG3_YAW.
 * 
 *
 * @group Sensor Calibration
 * @value -1 Internal
 * @value 0 No rotation
 * @value 1 Yaw 45°
 * @value 2 Yaw 90°
 * @value 3 Yaw 135°
 * @value 4 Yaw 180°
 * @value 5 Yaw 225°
 * @value 6 Yaw 270°
 * @value 7 Yaw 315°
 * @value 8 Roll 180°
 * @value 9 Roll 180°, Yaw 45°
 * @value 10 Roll 180°, Yaw 90°
 * @value 11 Roll 180°, Yaw 135°
 * @value 12 Pitch 180°
 * @value 13 Roll 180°, Yaw 225°
 * @value 14 Roll 180°, Yaw 270°
 * @value 15 Roll 180°, Yaw 315°
 * @value 16 Roll 90°
 * @value 17 Roll 90°, Yaw 45°
 * @value 18 Roll 90°, Yaw 90°
 * @value 19 Roll 90°, Yaw 135°
 * @value 20 Roll 270°
 * @value 21 Roll 270°, Yaw 45°
 * @value 22 Roll 270°, Yaw 90°
 * @value 23 Roll 270°, Yaw 135°
 * @value 24 Pitch 90°
 * @value 25 Pitch 270°
 * @value 26 Pitch 180°, Yaw 90°
 * @value 27 Pitch 180°, Yaw 270°
 * @value 28 Roll 90°, Pitch 90°
 * @value 29 Roll 180°, Pitch 90°
 * @value 30 Roll 270°, Pitch 90°
 * @value 31 Roll 90°, Pitch 180°
 * @value 32 Roll 270°, Pitch 180°
 * @value 33 Roll 90°, Pitch 270°
 * @value 34 Roll 180°, Pitch 270°
 * @value 35 Roll 270°, Pitch 270°
 * @value 36 Roll 90°, Pitch 180°, Yaw 90°
 * @value 37 Roll 90°, Yaw 270°
 * @value 38 Roll 90°, Pitch 68°, Yaw 293°
 * @value 39 Pitch 315°
 * @value 40 Roll 90°, Pitch 315°
 * @value 100 Custom Euler Angle
 * @category System
 * @min -1
 * @max 100
 */
PARAM_DEFINE_INT32(CAL_MAG3_ROT, -1);

/**
 * Magnetometer 0 Custom Euler Roll Angle
 *
 * Setting this parameter changes CAL_MAG0_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_ROLL, 0.0);

/**
 * Magnetometer 1 Custom Euler Roll Angle
 *
 * Setting this parameter changes CAL_MAG1_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_ROLL, 0.0);

/**
 * Magnetometer 2 Custom Euler Roll Angle
 *
 * Setting this parameter changes CAL_MAG2_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_ROLL, 0.0);

/**
 * Magnetometer 3 Custom Euler Roll Angle
 *
 * Setting this parameter changes CAL_MAG3_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_ROLL, 0.0);

/**
 * Magnetometer 0 Custom Euler Pitch Angle
 *
 * Setting this parameter changes CAL_MAG0_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_PITCH, 0.0);

/**
 * Magnetometer 1 Custom Euler Pitch Angle
 *
 * Setting this parameter changes CAL_MAG1_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_PITCH, 0.0);

/**
 * Magnetometer 2 Custom Euler Pitch Angle
 *
 * Setting this parameter changes CAL_MAG2_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_PITCH, 0.0);

/**
 * Magnetometer 3 Custom Euler Pitch Angle
 *
 * Setting this parameter changes CAL_MAG3_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_PITCH, 0.0);

/**
 * Magnetometer 0 Custom Euler Yaw Angle
 *
 * Setting this parameter changes CAL_MAG0_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_YAW, 0.0);

/**
 * Magnetometer 1 Custom Euler Yaw Angle
 *
 * Setting this parameter changes CAL_MAG1_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_YAW, 0.0);

/**
 * Magnetometer 2 Custom Euler Yaw Angle
 *
 * Setting this parameter changes CAL_MAG2_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_YAW, 0.0);

/**
 * Magnetometer 3 Custom Euler Yaw Angle
 *
 * Setting this parameter changes CAL_MAG3_ROT to "Custom Euler Angle"
 *
 * @group Sensor Calibration
 * @category System
 * @min -180
 * @max 180
 * @unit deg
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_YAW, 0.0);

/**
 * Magnetometer 0 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_XOFF, 0.0);

/**
 * Magnetometer 1 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_XOFF, 0.0);

/**
 * Magnetometer 2 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_XOFF, 0.0);

/**
 * Magnetometer 3 X-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_XOFF, 0.0);

/**
 * Magnetometer 0 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_YOFF, 0.0);

/**
 * Magnetometer 1 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_YOFF, 0.0);

/**
 * Magnetometer 2 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_YOFF, 0.0);

/**
 * Magnetometer 3 Y-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_YOFF, 0.0);

/**
 * Magnetometer 0 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_ZOFF, 0.0);

/**
 * Magnetometer 1 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_ZOFF, 0.0);

/**
 * Magnetometer 2 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_ZOFF, 0.0);

/**
 * Magnetometer 3 Z-axis offset
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @unit gauss
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_ZOFF, 0.0);

/**
 * Magnetometer 0 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_XSCALE, 1.0);

/**
 * Magnetometer 1 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_XSCALE, 1.0);

/**
 * Magnetometer 2 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_XSCALE, 1.0);

/**
 * Magnetometer 3 X-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_XSCALE, 1.0);

/**
 * Magnetometer 0 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_YSCALE, 1.0);

/**
 * Magnetometer 1 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_YSCALE, 1.0);

/**
 * Magnetometer 2 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_YSCALE, 1.0);

/**
 * Magnetometer 3 Y-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_YSCALE, 1.0);

/**
 * Magnetometer 0 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_ZSCALE, 1.0);

/**
 * Magnetometer 1 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_ZSCALE, 1.0);

/**
 * Magnetometer 2 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_ZSCALE, 1.0);

/**
 * Magnetometer 3 Z-axis scaling factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 * @min 0.1
 * @max 3.0
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_ZSCALE, 1.0);

/**
 * Magnetometer 0 X-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_XODIAG, 0.0);

/**
 * Magnetometer 1 X-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_XODIAG, 0.0);

/**
 * Magnetometer 2 X-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_XODIAG, 0.0);

/**
 * Magnetometer 3 X-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_XODIAG, 0.0);

/**
 * Magnetometer 0 Y-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_YODIAG, 0.0);

/**
 * Magnetometer 1 Y-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_YODIAG, 0.0);

/**
 * Magnetometer 2 Y-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_YODIAG, 0.0);

/**
 * Magnetometer 3 Y-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_YODIAG, 0.0);

/**
 * Magnetometer 0 Z-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_ZODIAG, 0.0);

/**
 * Magnetometer 1 Z-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_ZODIAG, 0.0);

/**
 * Magnetometer 2 Z-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_ZODIAG, 0.0);

/**
 * Magnetometer 3 Z-axis off diagonal scale factor
 *
 * 
 *
 * @group Sensor Calibration
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_ZODIAG, 0.0);

/**
 * Magnetometer 0 X Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * X component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_XCOMP, 0.0);

/**
 * Magnetometer 1 X Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * X component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_XCOMP, 0.0);

/**
 * Magnetometer 2 X Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * X component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_XCOMP, 0.0);

/**
 * Magnetometer 3 X Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * X component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_XCOMP, 0.0);

/**
 * Magnetometer 0 Y Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Y component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_YCOMP, 0.0);

/**
 * Magnetometer 1 Y Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Y component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_YCOMP, 0.0);

/**
 * Magnetometer 2 Y Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Y component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_YCOMP, 0.0);

/**
 * Magnetometer 3 Y Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Y component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_YCOMP, 0.0);

/**
 * Magnetometer 0 Z Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Z component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG0_ZCOMP, 0.0);

/**
 * Magnetometer 1 Z Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Z component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG1_ZCOMP, 0.0);

/**
 * Magnetometer 2 Z Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Z component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG2_ZCOMP, 0.0);

/**
 * Magnetometer 3 Z Axis throttle compensation
 *
 * Coefficient describing linear relationship between
 * Z component of magnetometer in body frame axis
 * and either current or throttle depending on value of CAL_MAG_COMP_TYP.
 * Unit for throttle-based compensation is [G] and
 * for current-based compensation [G/kA]
 * 
 *
 * @group Sensor Calibration
 * @decimal 3
 * @category System
 * @volatile True
 */
PARAM_DEFINE_FLOAT(CAL_MAG3_ZCOMP, 0.0);

/**
 * Servo 1 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA1, 45.0);

/**
 * Servo 2 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA2, 45.0);

/**
 * Servo 3 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA3, 45.0);

/**
 * Servo 4 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA4, 45.0);

/**
 * Servo 5 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA5, 45.0);

/**
 * Servo 6 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA6, 45.0);

/**
 * Servo 7 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA7, 45.0);

/**
 * Servo 8 Angle at Maximum
 *
 * Defines the angle when the servo is at the maximum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MAXA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MAXA8, 45.0);

/**
 * Servo 1 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA1, -45.0);

/**
 * Servo 2 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA2, -45.0);

/**
 * Servo 3 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA3, -45.0);

/**
 * Servo 4 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA4, -45.0);

/**
 * Servo 5 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA5, -45.0);

/**
 * Servo 6 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA6, -45.0);

/**
 * Servo 7 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA7, -45.0);

/**
 * Servo 8 Angle at Minimum
 *
 * Defines the angle when the servo is at the minimum.
 * Currently only supported in gz simulation and must be coherent with .sdf file and CA_SV_TL{n}_MINA.
 * 
 *
 * @group Geometry
 * @decimal 0
 * @min -180.0
 * @max 180.0
 * @unit deg
 */
PARAM_DEFINE_FLOAT(SIM_GZ_SV_MINA8, -45.0);

/**
 * SIM_GZ ESC 1 Output Function
 *
 * Select what should be output on SIM_GZ ESC 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC1, 0);

/**
 * SIM_GZ ESC 2 Output Function
 *
 * Select what should be output on SIM_GZ ESC 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC2, 0);

/**
 * SIM_GZ ESC 3 Output Function
 *
 * Select what should be output on SIM_GZ ESC 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC3, 0);

/**
 * SIM_GZ ESC 4 Output Function
 *
 * Select what should be output on SIM_GZ ESC 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC4, 0);

/**
 * SIM_GZ ESC 5 Output Function
 *
 * Select what should be output on SIM_GZ ESC 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC5, 0);

/**
 * SIM_GZ ESC 6 Output Function
 *
 * Select what should be output on SIM_GZ ESC 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC6, 0);

/**
 * SIM_GZ ESC 7 Output Function
 *
 * Select what should be output on SIM_GZ ESC 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC7, 0);

/**
 * SIM_GZ ESC 8 Output Function
 *
 * Select what should be output on SIM_GZ ESC 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC8, 0);

/**
 * SIM_GZ ESC 9 Output Function
 *
 * Select what should be output on SIM_GZ ESC 9.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC9, 0);

/**
 * SIM_GZ ESC 10 Output Function
 *
 * Select what should be output on SIM_GZ ESC 10.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC10, 0);

/**
 * SIM_GZ ESC 11 Output Function
 *
 * Select what should be output on SIM_GZ ESC 11.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC11, 0);

/**
 * SIM_GZ ESC 12 Output Function
 *
 * Select what should be output on SIM_GZ ESC 12.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC12, 0);

/**
 * SIM_GZ ESC 13 Output Function
 *
 * Select what should be output on SIM_GZ ESC 13.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC13, 0);

/**
 * SIM_GZ ESC 14 Output Function
 *
 * Select what should be output on SIM_GZ ESC 14.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC14, 0);

/**
 * SIM_GZ ESC 15 Output Function
 *
 * Select what should be output on SIM_GZ ESC 15.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC15, 0);

/**
 * SIM_GZ ESC 16 Output Function
 *
 * Select what should be output on SIM_GZ ESC 16.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FUNC16, 0);

/**
 * SIM_GZ ESC 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS1, 0);

/**
 * SIM_GZ ESC 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS2, 0);

/**
 * SIM_GZ ESC 3 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS3, 0);

/**
 * SIM_GZ ESC 4 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS4, 0);

/**
 * SIM_GZ ESC 5 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS5, 0);

/**
 * SIM_GZ ESC 6 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS6, 0);

/**
 * SIM_GZ ESC 7 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS7, 0);

/**
 * SIM_GZ ESC 8 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS8, 0);

/**
 * SIM_GZ ESC 9 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS9, 0);

/**
 * SIM_GZ ESC 10 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS10, 0);

/**
 * SIM_GZ ESC 11 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS11, 0);

/**
 * SIM_GZ ESC 12 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS12, 0);

/**
 * SIM_GZ ESC 13 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS13, 0);

/**
 * SIM_GZ ESC 14 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS14, 0);

/**
 * SIM_GZ ESC 15 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS15, 0);

/**
 * SIM_GZ ESC 16 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_DIS16, 0);

/**
 * SIM_GZ ESC 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN1, 0);

/**
 * SIM_GZ ESC 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN2, 0);

/**
 * SIM_GZ ESC 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN3, 0);

/**
 * SIM_GZ ESC 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN4, 0);

/**
 * SIM_GZ ESC 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN5, 0);

/**
 * SIM_GZ ESC 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN6, 0);

/**
 * SIM_GZ ESC 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN7, 0);

/**
 * SIM_GZ ESC 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN8, 0);

/**
 * SIM_GZ ESC 9 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN9, 0);

/**
 * SIM_GZ ESC 10 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN10, 0);

/**
 * SIM_GZ ESC 11 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN11, 0);

/**
 * SIM_GZ ESC 12 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN12, 0);

/**
 * SIM_GZ ESC 13 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN13, 0);

/**
 * SIM_GZ ESC 14 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN14, 0);

/**
 * SIM_GZ ESC 15 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN15, 0);

/**
 * SIM_GZ ESC 16 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MIN16, 0);

/**
 * SIM_GZ ESC 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX1, 1000);

/**
 * SIM_GZ ESC 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX2, 1000);

/**
 * SIM_GZ ESC 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX3, 1000);

/**
 * SIM_GZ ESC 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX4, 1000);

/**
 * SIM_GZ ESC 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX5, 1000);

/**
 * SIM_GZ ESC 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX6, 1000);

/**
 * SIM_GZ ESC 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX7, 1000);

/**
 * SIM_GZ ESC 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX8, 1000);

/**
 * SIM_GZ ESC 9 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX9, 1000);

/**
 * SIM_GZ ESC 10 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX10, 1000);

/**
 * SIM_GZ ESC 11 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX11, 1000);

/**
 * SIM_GZ ESC 12 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX12, 1000);

/**
 * SIM_GZ ESC 13 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX13, 1000);

/**
 * SIM_GZ ESC 14 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX14, 1000);

/**
 * SIM_GZ ESC 15 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX15, 1000);

/**
 * SIM_GZ ESC 16 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_MAX16, 1000);

/**
 * SIM_GZ ESC 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL1, -1);

/**
 * SIM_GZ ESC 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL2, -1);

/**
 * SIM_GZ ESC 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL3, -1);

/**
 * SIM_GZ ESC 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL4, -1);

/**
 * SIM_GZ ESC 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL5, -1);

/**
 * SIM_GZ ESC 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL6, -1);

/**
 * SIM_GZ ESC 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL7, -1);

/**
 * SIM_GZ ESC 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL8, -1);

/**
 * SIM_GZ ESC 9 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC9).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL9, -1);

/**
 * SIM_GZ ESC 10 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC10).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL10, -1);

/**
 * SIM_GZ ESC 11 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC11).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL11, -1);

/**
 * SIM_GZ ESC 12 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC12).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL12, -1);

/**
 * SIM_GZ ESC 13 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC13).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL13, -1);

/**
 * SIM_GZ ESC 14 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC14).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL14, -1);

/**
 * SIM_GZ ESC 15 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC15).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL15, -1);

/**
 * SIM_GZ ESC 16 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_EC_FUNC16).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_FAIL16, -1);

/**
 * SIM_GZ Servo 1 Output Function
 *
 * Select what should be output on SIM_GZ Servo 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC1, 0);

/**
 * SIM_GZ Servo 2 Output Function
 *
 * Select what should be output on SIM_GZ Servo 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC2, 0);

/**
 * SIM_GZ Servo 3 Output Function
 *
 * Select what should be output on SIM_GZ Servo 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC3, 0);

/**
 * SIM_GZ Servo 4 Output Function
 *
 * Select what should be output on SIM_GZ Servo 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC4, 0);

/**
 * SIM_GZ Servo 5 Output Function
 *
 * Select what should be output on SIM_GZ Servo 5.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC5, 0);

/**
 * SIM_GZ Servo 6 Output Function
 *
 * Select what should be output on SIM_GZ Servo 6.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC6, 0);

/**
 * SIM_GZ Servo 7 Output Function
 *
 * Select what should be output on SIM_GZ Servo 7.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC7, 0);

/**
 * SIM_GZ Servo 8 Output Function
 *
 * Select what should be output on SIM_GZ Servo 8.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FUNC8, 0);

/**
 * SIM_GZ Servo 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS1, 500);

/**
 * SIM_GZ Servo 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS2, 500);

/**
 * SIM_GZ Servo 3 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS3, 500);

/**
 * SIM_GZ Servo 4 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS4, 500);

/**
 * SIM_GZ Servo 5 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS5, 500);

/**
 * SIM_GZ Servo 6 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS6, 500);

/**
 * SIM_GZ Servo 7 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS7, 500);

/**
 * SIM_GZ Servo 8 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_DIS8, 500);

/**
 * SIM_GZ Servo 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN1, 0);

/**
 * SIM_GZ Servo 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN2, 0);

/**
 * SIM_GZ Servo 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN3, 0);

/**
 * SIM_GZ Servo 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN4, 0);

/**
 * SIM_GZ Servo 5 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN5, 0);

/**
 * SIM_GZ Servo 6 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN6, 0);

/**
 * SIM_GZ Servo 7 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN7, 0);

/**
 * SIM_GZ Servo 8 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MIN8, 0);

/**
 * SIM_GZ Servo 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX1, 1000);

/**
 * SIM_GZ Servo 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX2, 1000);

/**
 * SIM_GZ Servo 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX3, 1000);

/**
 * SIM_GZ Servo 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX4, 1000);

/**
 * SIM_GZ Servo 5 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX5, 1000);

/**
 * SIM_GZ Servo 6 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX6, 1000);

/**
 * SIM_GZ Servo 7 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX7, 1000);

/**
 * SIM_GZ Servo 8 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_MAX8, 1000);

/**
 * SIM_GZ Servo 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL1, -1);

/**
 * SIM_GZ Servo 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL2, -1);

/**
 * SIM_GZ Servo 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL3, -1);

/**
 * SIM_GZ Servo 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL4, -1);

/**
 * SIM_GZ Servo 5 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC5).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL5, -1);

/**
 * SIM_GZ Servo 6 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC6).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL6, -1);

/**
 * SIM_GZ Servo 7 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC7).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL7, -1);

/**
 * SIM_GZ Servo 8 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_SV_FUNC8).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 1000
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_FAIL8, -1);

/**
 * SIM_GZ Wheels 1 Output Function
 *
 * Select what should be output on SIM_GZ Wheels 1.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FUNC1, 0);

/**
 * SIM_GZ Wheels 2 Output Function
 *
 * Select what should be output on SIM_GZ Wheels 2.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FUNC2, 0);

/**
 * SIM_GZ Wheels 3 Output Function
 *
 * Select what should be output on SIM_GZ Wheels 3.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FUNC3, 0);

/**
 * SIM_GZ Wheels 4 Output Function
 *
 * Select what should be output on SIM_GZ Wheels 4.
 * 
 * The default failsafe value is set according to the selected function:
 * - 'Min' for ConstantMin
 * - 'Max' for ConstantMax
 * - 'Max' for Parachute
 * - ('Max'+'Min')/2 for Servos
 * - 'Disarmed' for the rest
 * 
 *
 * @group Actuator Outputs
 * @value 0 Disabled
 * @value 1 Constant Min
 * @value 2 Constant Max
 * @value 101 Motor 1
 * @value 102 Motor 2
 * @value 103 Motor 3
 * @value 104 Motor 4
 * @value 105 Motor 5
 * @value 106 Motor 6
 * @value 107 Motor 7
 * @value 108 Motor 8
 * @value 109 Motor 9
 * @value 110 Motor 10
 * @value 111 Motor 11
 * @value 112 Motor 12
 * @value 201 Servo 1
 * @value 202 Servo 2
 * @value 203 Servo 3
 * @value 204 Servo 4
 * @value 205 Servo 5
 * @value 206 Servo 6
 * @value 207 Servo 7
 * @value 208 Servo 8
 * @value 301 Peripheral via Actuator Set 1
 * @value 302 Peripheral via Actuator Set 2
 * @value 303 Peripheral via Actuator Set 3
 * @value 304 Peripheral via Actuator Set 4
 * @value 305 Peripheral via Actuator Set 5
 * @value 306 Peripheral via Actuator Set 6
 * @value 400 Landing Gear
 * @value 401 Parachute
 * @value 402 RC Roll
 * @value 403 RC Pitch
 * @value 404 RC Throttle
 * @value 405 RC Yaw
 * @value 406 RC Flaps
 * @value 407 RC AUX 1
 * @value 408 RC AUX 2
 * @value 409 RC AUX 3
 * @value 410 RC AUX 4
 * @value 411 RC AUX 5
 * @value 412 RC AUX 6
 * @value 420 Gimbal Roll
 * @value 421 Gimbal Pitch
 * @value 422 Gimbal Yaw
 * @value 430 Gripper
 * @value 440 Landing Gear Wheel
 * @value 450 IC Engine Ignition
 * @value 451 IC Engine Throttle
 * @value 452 IC Engine Choke
 * @value 453 IC Engine Starter
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FUNC4, 0);

/**
 * SIM_GZ Wheels 1 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_DIS1, 100);

/**
 * SIM_GZ Wheels 2 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_DIS2, 100);

/**
 * SIM_GZ Wheels 3 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_DIS3, 100);

/**
 * SIM_GZ Wheels 4 Disarmed Value
 *
 * This is the output value that is set when not armed.
 * 
 * Note that non-motor outputs might already be active in prearm state if COM_PREARM_MODE is set.
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_DIS4, 100);

/**
 * SIM_GZ Wheels 1 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MIN1, 0);

/**
 * SIM_GZ Wheels 2 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MIN2, 0);

/**
 * SIM_GZ Wheels 3 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MIN3, 0);

/**
 * SIM_GZ Wheels 4 Minimum Value
 *
 * Minimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MIN4, 0);

/**
 * SIM_GZ Wheels 1 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MAX1, 200);

/**
 * SIM_GZ Wheels 2 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MAX2, 200);

/**
 * SIM_GZ Wheels 3 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MAX3, 200);

/**
 * SIM_GZ Wheels 4 Maximum Value
 *
 * Maxmimum output value (when not disarmed).
 * 
 *
 * @group Actuator Outputs
 * @min 0
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_MAX4, 200);

/**
 * SIM_GZ Wheels 1 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_WH_FUNC1).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FAIL1, -1);

/**
 * SIM_GZ Wheels 2 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_WH_FUNC2).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FAIL2, -1);

/**
 * SIM_GZ Wheels 3 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_WH_FUNC3).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FAIL3, -1);

/**
 * SIM_GZ Wheels 4 Failsafe Value
 *
 * This is the output value that is set when in failsafe mode.
 * 
 * When set to -1 (default), the value depends on the function (see SIM_GZ_WH_FUNC4).
 * 
 *
 * @group Actuator Outputs
 * @min -1
 * @max 200
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_FAIL4, -1);

/**
 * Reverse Output Range for SIM_GZ
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 SIM_GZ ESC 1
 * @bit 1 SIM_GZ ESC 2
 * @bit 2 SIM_GZ ESC 3
 * @bit 3 SIM_GZ ESC 4
 * @bit 4 SIM_GZ ESC 5
 * @bit 5 SIM_GZ ESC 6
 * @bit 6 SIM_GZ ESC 7
 * @bit 7 SIM_GZ ESC 8
 * @bit 8 SIM_GZ ESC 9
 * @bit 9 SIM_GZ ESC 10
 * @bit 10 SIM_GZ ESC 11
 * @bit 11 SIM_GZ ESC 12
 * @bit 12 SIM_GZ ESC 13
 * @bit 13 SIM_GZ ESC 14
 * @bit 14 SIM_GZ ESC 15
 * @bit 15 SIM_GZ ESC 16
 * @min 0
 * @max 65535
 */
PARAM_DEFINE_INT32(SIM_GZ_EC_REV, 0);

/**
 * Reverse Output Range for SIM_GZ
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 SIM_GZ Servo 1
 * @bit 1 SIM_GZ Servo 2
 * @bit 2 SIM_GZ Servo 3
 * @bit 3 SIM_GZ Servo 4
 * @bit 4 SIM_GZ Servo 5
 * @bit 5 SIM_GZ Servo 6
 * @bit 6 SIM_GZ Servo 7
 * @bit 7 SIM_GZ Servo 8
 * @min 0
 * @max 255
 */
PARAM_DEFINE_INT32(SIM_GZ_SV_REV, 0);

/**
 * Reverse Output Range for SIM_GZ
 *
 * Allows to reverse the output range for each channel.
 * Note: this is only useful for servos.
 * 
 *
 * @group Actuator Outputs
 * @bit 0 SIM_GZ Wheels 1
 * @bit 1 SIM_GZ Wheels 2
 * @bit 2 SIM_GZ Wheels 3
 * @bit 3 SIM_GZ Wheels 4
 * @min 0
 * @max 15
 */
PARAM_DEFINE_INT32(SIM_GZ_WH_REV, 0);

/**
 * uXRCE-DDS domain ID
 *
 * uXRCE-DDS domain ID
 *
 * @group UXRCE-DDS Client
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_DOM_ID, 0);

/**
 * uXRCE-DDS session key
 *
 * uXRCE-DDS key, must be different from zero.
 * In a single agent - multi client configuration, each client
 * must have a unique session key.
 * 
 *
 * @group UXRCE-DDS Client
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_KEY, 1);

/**
 * uXRCE-DDS participant configuration
 *
 * Set the participant configuration on the Agent's system.
 * 0: Use the default configuration.
 * 1: Restrict messages to localhost
 *    (use in combination with ROS_LOCALHOST_ONLY=1).
 * 2: Use a custom participant with the profile name "px4_participant".
 * 
 *
 * @group UXRCE-DDS Client
 * @value 0 Default
 * @value 1 Localhost-only
 * @value 2 Custom participant
 * @category System
 * @min 0
 * @max 2
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_PTCFG, 0);

/**
 * Enable uXRCE-DDS timestamp synchronization
 *
 * When enabled, uxrce_dds_client will synchronize the timestamps of the incoming and outgoing messages measuring the offset between the Agent OS time and the PX4 time.
 *
 * @group UXRCE-DDS Client
 * @boolean
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_SYNCT, 1);

/**
 * Enable uXRCE-DDS system clock synchronization
 *
 * When enabled along with UXRCE_DDS_SYNCT, uxrce_dds_client will set the system clock using the agents UTC timestamp.
 *
 * @group UXRCE-DDS Client
 * @boolean
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_SYNCC, 0);

/**
 * TX rate timeout configuration
 *
 * Specifies after how many seconds without sending data the DDS connection is reestablished.
 * A value less than one disables the TX rate timeout.
 * 
 *
 * @group UXRCE-DDS Client
 * @category System
 * @unit s
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_TX_TO, 3);

/**
 * RX rate timeout configuration
 *
 * Specifies after how many seconds without receiving data the DDS connection is reestablished.
 * A value less than one disables the RX rate timeout.
 * 
 *
 * @group UXRCE-DDS Client
 * @category System
 * @unit s
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_RX_TO, -1);

/**
 * Define an index-based message namespace
 *
 * Defines an index-based namespace for DDS messages, e.g, uav_0, uav_1, up to uav_9999
 * A value less than zero leaves the namespace empty
 * 
 *
 * @group UXRCE-DDS Client
 * @category System
 * @min -1
 * @max 9999
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_NS_IDX, -1);

/**
 * Enable serial flow control for UXRCE interface
 *
 * This is used to enable flow control for the serial uXRCE instance.
 * Used for reliable high bandwidth communication.
 * 
 *
 * @group UXRCE-DDS Client
 * @boolean
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(UXRCE_DDS_FLCTRL, 0);

/**
 * Enable Zenoh
 *
 * Set true (1) to start the Zenoh driver module (a.k.a the "Zenoh-Pico Node").
 * See https://docs.px4.io/main/en/middleware/zenoh and
 * https://docs.px4.io/main/en/modules/modules_driver.html#zenoh
 * 
 *
 * @group Zenoh
 * @boolean
 * @category System
 * @reboot_required True
 */
PARAM_DEFINE_INT32(ZENOH_ENABLE, 0);
