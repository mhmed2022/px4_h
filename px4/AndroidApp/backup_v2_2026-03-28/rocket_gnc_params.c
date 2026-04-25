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
 * @file rocket_gnc_params.c
 * Parameters for Rocket M130 GNC module.
 */

#include <px4_platform_common/px4_config.h>
#include <parameters/param.h>

/**
 * Rocket Stage-1 target altitude
 *
 * Target altitude for stage 1 altitude-hold controller.
 *
 * @unit m
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_SET_ALT, 100.0f);

/**
 * Rocket Stage-1 end time
 *
 * Fallback time limit for stage 1. Transition to stage 2 occurs
 * when |yaw_los_deg| > 11 deg OR t >= this value.
 *
 * @unit s
 * @min 0.5
 * @max 30.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_T_STG1, 4.0f);

/**
 * Rocket control activation delay
 *
 * Time after launch before control surfaces are activated.
 * Corresponds to launcher rail time.
 *
 * @unit s
 * @min 0.0
 * @max 5.0
 * @decimal 2
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_T_CTRL, 0.1f);

/**
 * Proportional navigation gain (Npn)
 *
 * Navigation ratio for biased proportional navigation (pn2).
 *
 * @min 1.0
 * @max 6.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_NPN, 2.7f);

/**
 * Terminal impact angle
 *
 * Desired impact angle at target. Negative = diving down.
 *
 * @unit deg
 * @min -90.0
 * @max 0.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_IMP_ANG, -30.0f);

/**
 * PN bias time constant (tau_pn1)
 *
 * Time constant for bias integrator in pn2 guidance.
 *
 * @unit s
 * @min 1.0
 * @max 60.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_TAU_PN1, 20.0f);

/**
 * Yaw guidance gain (k_guidance_yaw)
 *
 * Gain for yaw command correction in yaw_comm (t < 5s).
 *
 * @min 0.001
 * @max 0.1
 * @decimal 4
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_K_YAW, 0.008f);

/**
 * Yaw velocity feedback gain (k_vz)
 *
 * Velocity feedback gain in yaw_comm (t < 5s).
 *
 * @min 1.0
 * @max 50.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_K_VZ, 14.0f);

/**
 * Yaw acceleration command limit
 *
 * Maximum yaw acceleration command from guidance.
 *
 * @unit norm
 * @min 0.5
 * @max 10.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_AYC_LIM, 2.0f);

/**
 * Pitch acceleration command limit
 *
 * Maximum pitch acceleration command from guidance (pn2 saturation).
 *
 * @unit norm
 * @min 1.0
 * @max 20.0
 * @decimal 1
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_APC_LIM, 8.0f);

/**
 * Target X position (Forward/North)
 *
 * @unit m
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_XTRGT, 2600.0f);

/**
 * Target Y position (Up)
 *
 * @unit m
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_YTRGT, 0.0f);

/**
 * Target Z position (East)
 *
 * @unit m
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_ZTRGT, 0.0f);

/**
 * Maximum fin deflection
 *
 * Physical limit of fin servo deflection.
 *
 * @unit rad
 * @min 0.1
 * @max 0.5
 * @decimal 3
 * @group Rocket GNC
 */
PARAM_DEFINE_FLOAT(ROCKET_MAX_DEFL, 0.436f);

/**
 * Ground test mode
 *
 * When enabled, overrides dynamic pressure with a fixed value after
 * launch detection so that fins respond on the ground.
 * MUST be set to 0 for real flight.
 *
 * @boolean
 * @group Rocket GNC
 */
PARAM_DEFINE_INT32(ROCKET_GND_TEST, 0);
