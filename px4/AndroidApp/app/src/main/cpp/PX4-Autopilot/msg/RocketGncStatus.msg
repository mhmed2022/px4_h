# Rocket GNC Status
# Published by rocket_gnc module for logging and QGroundControl telemetry

uint64 timestamp          # [us] Time since system start
uint64 timestamp_sample   # [us] Timestamp of input data

uint8 stage               # Flight stage (1=altitude hold, 2=APC, 3=late)
float32 t_flight          # [s] Time since launch detection
float32 q_dyn             # [Pa] Dynamic pressure
float32 q_kgf             # [kgf/cm2] Dynamic pressure scaled

float32 pitch_accel_cmd   # [g] Pitch acceleration command from guidance
float32 yaw_accel_cmd     # [g] Yaw acceleration command from guidance
float32 yaw_los_deg       # [deg] Yaw line-of-sight angle

float32 delta_roll        # [rad] Roll channel command
float32 delta_pitch       # [rad] Pitch channel command
float32 delta_yaw         # [rad] Yaw channel command

float32 fin1              # [-1,+1] Fin 1 normalized deflection
float32 fin2              # [-1,+1] Fin 2 normalized deflection
float32 fin3              # [-1,+1] Fin 3 normalized deflection
float32 fin4              # [-1,+1] Fin 4 normalized deflection

float32 altitude          # [m] Current altitude
float32 airspeed          # [m/s] Current airspeed
float32 rho               # [kg/m3] Air density

float32 phi               # [rad] Actual roll angle
float32 wx_filter         # [rad/s] Filtered roll rate
float32 du_roll           # [rad] Roll error (phic - phi, clamped)
float32 out_integ_roll    # Roll integrator output

uint8 servo_online_mask   # Bitmask: bit0=servo0, bit1=servo1, bit2=servo2, bit3=servo3 (1=online, 0=offline)

# --- Attitude angles ---
float32 theta                    # [rad] Pitch angle
float32 psi                      # [rad] Yaw/heading angle
float32 alpha_est                # [rad] Estimated angle of attack
float32 gamma_rad                # [rad] Flight path angle

# --- Position in guidance frame ---
float32 pos_downrange            # [m] Downrange position (Xm)
float32 pos_crossrange           # [m] Crossrange position (Zm)
float32 vel_downrange            # [m/s] Downrange velocity (Vxm)
float32 vel_down                 # [m/s] Down velocity (Vym)
float32 vel_crossrange           # [m/s] Crossrange velocity (Vzm)

# --- Controller internal state ---
float32 wz_filter                # [rad/s] Filtered pitch rate
float32 wy_filter                # [rad/s] Filtered yaw rate
float32 ayb_filter               # [g] Filtered lateral body acceleration
float32 azb_filter               # [g] Filtered normal body acceleration
float32 out_integ_yaw            # Yaw integrator output
float32 apc_accum                # Pitch acceleration integrator

# --- PN guidance state ---
float32 pn2_b1                   # PN bias rate
float32 pn2_b_int                # PN bias integral
float32 bearing_deg              # [deg] Bearing to target from arm heading
float32 target_range_remaining   # [m] Remaining range to target

# --- Event flags ---
bool alpha_protection_active     # True when alpha protection is active
bool launched                    # True after launch detection
