// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <AP_Common/AP_Common.h>
#include <AP_Param/AP_Param.h>
#include <AC_PID/AC_PID.h>
#include <RC_Channel/RC_Channel.h>
#include <APM_PI/APM_PI.h>
#include <AP_Camera/AP_Camera.h>
#include <AP_Relay/AP_Relay.h>
#include <AP_RangeFinder/AP_RangeFinder_MaxsonarXL.h>
#include <AP_Compass/AP_Compass.h>
#include <AP_InertialSensor/AP_InertialSensor.h>
#include <AP_InertialNav/AP_InertialNav.h>
#include <AC_WPNav/AC_WPNav.h>
#include "GCS.h"
#include <AP_Mount/AP_Mount.h>
#include <AP_BattMonitor/AP_BattMonitor.h>
#include <AP_Scheduler/AP_Scheduler.h>
#include <AP_Notify/AP_Notify.h>
#include <AC_Fence/AC_Fence.h>
#include <AP_Motors/AP_Motors.h>
#include <AP_RCMapper/AP_RCMapper.h>
#include <Arduino.h>
#include "config.h"

// Global parameter class.
//
class Parameters {
public:
	// The version of the layout as described by the parameter enum.
	//
	// When changing the parameter enum in an incompatible fashion, this
	// value should be incremented by one.
	//
	// The increment will prevent old parameters from being used incorrectly
	// by newer code.
	//
	static const uint16_t k_format_version = 120;

	// The parameter software_type is set up solely for ground station use
	// and identifies the software type (eg ArduPilotMega versus
	// ArduCopterMega)
	// GCS will interpret values 0-9 as ArduPilotMega.  Developers may use
	// values within that range to identify different branches.
	//
	static const uint16_t k_software_type = 10;          // 0 for APM
	                                                     // trunk

	                                                     // Parameter identities.
	                                                     //
	                                                     // The enumeration defined here is used to ensure that every parameter
	                                                     // or parameter group has a unique ID number.	This number is used by
	                                                     // AP_Param to store and locate parameters in EEPROM.
	                                                     //
	                                                     // Note that entries without a number are assigned the next number after
	                                                     // the entry preceding them.	When adding new entries, ensure that they
	                                                     // don't overlap.
	                                                     //
	                                                     // Try to group related variables together, and assign them a set
	                                                     // range in the enumeration.	Place these groups in numerical order
	                                                     // at the end of the enumeration.
	                                                     //
	                                                     // WARNING: Care should be taken when editing this enumeration as the
	                                                     //			AP_Param load/save code depends on the values here to identify
	                                                     //			variables saved in EEPROM.
	                                                     //
	                                                     //
	enum {
		// Layout version number, always key zero.
		//
		k_param_format_version = 0,
		k_param_software_type,
		k_param_ins_old,                        // *** Deprecated, remove with next eeprom number change
		k_param_ins,                            // libraries/AP_InertialSensor variables

		// simulation
		k_param_sitl = 10,

		// barometer object (needed for SITL)
		k_param_barometer,

		// scheduler object (for debugging)
		k_param_scheduler,

		// relay object
		k_param_relay,

		// Misc
		//
		k_param_log_bitmask = 20,
		k_param_log_last_filenumber,            // *** Deprecated - remove
		                                        // with next eeprom number
		                                        // change
		k_param_toy_yaw_rate,                   // deprecated - remove
		k_param_crosstrack_min_distance,	// deprecated - remove with next eeprom number change
		k_param_rssi_pin,
		k_param_throttle_accel_enabled,     // deprecated - remove
		k_param_wp_yaw_behavior,
		k_param_acro_trainer,
		k_param_pilot_velocity_z_max,
		k_param_circle_rate,
		k_param_sonar_gain,
		k_param_ch8_option,
		k_param_arming_check_enabled,
		k_param_sprayer,
		k_param_angle_max,
		k_param_gps_hdop_good,
		k_param_battery,
		k_param_fs_batt_mah,
		k_param_angle_rate_max,         // 38

		// 65: AP_Limits Library
		k_param_limits = 65,            // deprecated - remove
		k_param_gpslock_limit,          // deprecated - remove
		k_param_geofence_limit,         // deprecated - remove
		k_param_altitude_limit,         // deprecated - remove
		k_param_fence,
		k_param_gps_glitch,             // 70

		//
		// 75: Singlecopter
		//
		k_param_single_servo_1 = 75,
		k_param_single_servo_2,
		k_param_single_servo_3,
		k_param_single_servo_4, // 78

		//
		// 80: Heli
		//
		k_param_heli_servo_1 = 80,
		k_param_heli_servo_2,
		k_param_heli_servo_3,
		k_param_heli_servo_4,
		k_param_heli_pitch_ff,
		k_param_heli_roll_ff,
		k_param_heli_yaw_ff,
		k_param_heli_stab_col_min,
		k_param_heli_stab_col_max,  // 88

		//
		// 90: Motors
		//
		k_param_motors = 90,

		//
		// 100: Inertial Nav
		//
		k_param_inertial_nav = 100,
		k_param_wp_nav = 101,

		// 110: Telemetry control
		//
		k_param_gcs0 = 110,
		k_param_gcs3,
		k_param_sysid_this_mav,
		k_param_sysid_my_gcs,
		k_param_serial3_baud,
		k_param_telem_delay,

		//
		// 140: Sensor parameters
		//
		k_param_imu = 140, // deprecated - can be deleted
		k_param_battery_monitoring = 141,   // deprecated - can be deleted
		k_param_volt_div_ratio, // deprecated - can be deleted
		k_param_curr_amp_per_volt,  // deprecated - can be deleted
		k_param_input_voltage,  // deprecated - can be deleted
		k_param_pack_capacity,  // deprecated - can be deleted
		k_param_compass_enabled,
		k_param_compass,
		k_param_sonar_enabled,
		k_param_frame_orientation,
		k_param_optflow_enabled,
		k_param_fs_batt_voltage,
		k_param_ch7_option,
		k_param_auto_slew_rate,     // deprecated - can be deleted
		k_param_sonar_type,
		k_param_super_simple = 155,
		k_param_axis_enabled = 157, // deprecated - remove with next eeprom number change
		k_param_copter_leds_mode,
		k_param_ahrs, // AHRS group // 159

		//
		// 160: Navigation parameters
		//
		k_param_rtl_altitude = 160,
		k_param_crosstrack_gain,	// deprecated - remove with next eeprom number change
		k_param_rtl_loiter_time,
		k_param_rtl_alt_final,
		k_param_tilt_comp, 	//164	deprecated - remove with next eeprom number change

		//
		// Camera and mount parameters
		//
		k_param_camera = 165,
		k_param_camera_mount,
		k_param_camera_mount2,

		//
		// Batery monitoring parameters
		//
		k_param_battery_volt_pin = 168, // deprecated - can be deleted
		k_param_battery_curr_pin,   // 169 deprecated - can be deleted

		//
		// 170: Radio settings
		//
		k_param_rc_1 = 170,
		k_param_rc_2,
		k_param_rc_3,
		k_param_rc_4,
		k_param_rc_5,
		k_param_rc_6,
		k_param_rc_7,
		k_param_rc_8,
		k_param_rc_10,
		k_param_rc_11,
		k_param_throttle_min,
		k_param_throttle_max,
		k_param_failsafe_throttle,
		k_param_throttle_fs_action,     // remove
		k_param_failsafe_throttle_value,
		k_param_throttle_cruise,
		k_param_esc_calibrate,
		k_param_radio_tuning,
		k_param_radio_tuning_high,
		k_param_radio_tuning_low,
		k_param_rc_speed = 192,
		k_param_failsafe_battery_enabled,
		k_param_throttle_mid,
		k_param_failsafe_gps_enabled,
		k_param_rc_9,
		k_param_rc_12,
		k_param_failsafe_gcs,           // 198
		k_param_rcmap,

		//
		// 200: flight modes
		//
		k_param_flight_mode1 = 200,
		k_param_flight_mode2,
		k_param_flight_mode3,
		k_param_flight_mode4,
		k_param_flight_mode5,
		k_param_flight_mode6,
		k_param_simple_modes,

		//
		// 210: Waypoint data
		//
		k_param_waypoint_mode = 210, // remove
		k_param_command_total,
		k_param_command_index,
		k_param_command_nav_index,   // remove
		k_param_waypoint_radius,     // remove
		k_param_circle_radius,
		k_param_waypoint_speed_max,  // remove
		k_param_land_speed,
		k_param_auto_velocity_z_min, // remove
		k_param_auto_velocity_z_max, // remove - 219

		//
		// 220: PI/D Controllers
		//
		k_param_acro_rp_p = 221,
		k_param_axis_lock_p,    // remove
		k_param_pid_rate_roll,
		k_param_pid_rate_pitch,
		k_param_pid_rate_yaw,
		k_param_pi_stabilize_roll,
		k_param_pi_stabilize_pitch,
		k_param_pi_stabilize_yaw,
		k_param_pi_loiter_lat,
		k_param_pi_loiter_lon,
		k_param_pid_loiter_rate_lat,
		k_param_pid_loiter_rate_lon,
		k_param_pid_nav_lat,        // 233 - remove
		k_param_pid_nav_lon,        // 234 - remove
		k_param_pi_alt_hold,
		k_param_pid_throttle_rate,
		k_param_pid_optflow_roll,
		k_param_pid_optflow_pitch,
		k_param_acro_balance_roll_old,  // 239 - remove
		k_param_acro_balance_pitch_old, // 240 - remove
		k_param_pid_throttle_accel,
		k_param_acro_balance_roll,
		k_param_acro_balance_pitch,
		k_param_acro_yaw_p, // 244

	// 254,255: reserved
	};

	AP_Int16 format_version;
	AP_Int8 software_type;

	// Telemetry control
	//
	AP_Int16 sysid_this_mav;
	AP_Int16 sysid_my_gcs;
	AP_Int8 serial3_baud;
	AP_Int8 telem_delay;

	AP_Int16 rtl_altitude;
	AP_Int8 sonar_enabled;
	AP_Int8 sonar_type;       // 0 = XL, 1 = LV,
	                          // 2 = XLL (XL with 10m range)
	                          // 3 = HRLV
	AP_Float sonar_gain;

	AP_Int8 failsafe_battery_enabled;   // battery failsafe enabled
	AP_Float fs_batt_voltage;            // battery voltage below which failsafe will be triggered
	AP_Float fs_batt_mah;                // battery capacity (in mah) below which failsafe will be triggered

	AP_Int8 failsafe_gps_enabled;       // gps failsafe enabled
	AP_Int8 failsafe_gcs;               // ground station failsafe behavior
	AP_Int16 gps_hdop_good;              // GPS Hdop value at or below this value represent a good position

	AP_Int8 compass_enabled;
	AP_Int8 optflow_enabled;
	AP_Int8 super_simple;
	AP_Int16 rtl_alt_final;
	AP_Int8 copter_leds_mode;           // Operating mode of LED
	                                    // lighting system

	AP_Int8 rssi_pin;
	AP_Int8 wp_yaw_behavior;            // controls how the autopilot controls yaw during missions
	AP_Int16 angle_max;                  // maximum lean angle of the copter in centi-degrees
	AP_Int32 angle_rate_max;             // maximum rotation rate in roll/pitch axis requested by angle controller used in stabilize, loiter, rtl, auto flight modes

	// Waypoints
	//
	AP_Int8 command_total;
	AP_Int8 command_index;
	AP_Int16 circle_radius;
	AP_Float circle_rate;                // Circle mode's turn rate in deg/s.  positive to rotate clockwise, negative for counter clockwise
	AP_Int32 rtl_loiter_time;
	AP_Int16 land_speed;
	AP_Int16 pilot_velocity_z_max;        // maximum vertical velocity the pilot may request

	// Throttle
	//
	AP_Int16 throttle_min;
	AP_Int16 throttle_max;
	AP_Int8 failsafe_throttle;
	AP_Int16 failsafe_throttle_value;
	AP_Int16 throttle_cruise;
	AP_Int16 throttle_mid;

	// Flight modes
	//
	AP_Int8 flight_mode1;
	AP_Int8 flight_mode2;
	AP_Int8 flight_mode3;
	AP_Int8 flight_mode4;
	AP_Int8 flight_mode5;
	AP_Int8 flight_mode6;
	AP_Int8 simple_modes;

	// Misc
	//
	AP_Int16 log_bitmask;
	AP_Int8 esc_calibrate;
	AP_Int8 radio_tuning;
	AP_Int16 radio_tuning_high;
	AP_Int16 radio_tuning_low;
	AP_Int8 frame_orientation;
	AP_Int8 ch7_option;
	AP_Int8 ch8_option;
	AP_Int8 arming_check_enabled;

#if FRAME_CONFIG ==     HELI_FRAME
	// Heli
	RC_Channel heli_servo_1, heli_servo_2, heli_servo_3, heli_servo_4;// servos for swash plate and tail
	AP_Float heli_pitch_ff;// pitch rate feed-forward
	AP_Float heli_roll_ff;// roll rate feed-forward
	AP_Float heli_yaw_ff;// yaw rate feed-forward
	AP_Int16 heli_stab_col_min;// min collective while pilot directly controls collective in stabilize mode
	AP_Int16 heli_stab_col_max;// min collective while pilot directly controls collective in stabilize mode
#endif
#if FRAME_CONFIG ==     SINGLE_FRAME
	// Single
	RC_Channel single_servo_1, single_servo_2, single_servo_3, single_servo_4;// servos for four flaps
#endif

	// RC channels
	RC_Channel rc_1;
	RC_Channel rc_2;
	RC_Channel rc_3;
	RC_Channel rc_4;
	RC_Channel_aux rc_5;
	RC_Channel_aux rc_6;
	RC_Channel_aux rc_7;
	RC_Channel_aux rc_8;
	RC_Channel_aux rc_10;
	RC_Channel_aux rc_11;

#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
	RC_Channel_aux rc_9;
	RC_Channel_aux rc_12;
#endif

	AP_Int16 rc_speed; // speed of fast RC Channels in Hz

	// Acro parameters
	AP_Float acro_rp_p;
	AP_Float acro_yaw_p;
	AP_Float acro_balance_roll;
	AP_Float acro_balance_pitch;
	AP_Int8 acro_trainer;

	// PI/D controllers
	AC_PID pid_rate_roll;
	AC_PID pid_rate_pitch;
	AC_PID pid_rate_yaw;
	AC_PID pid_loiter_rate_lat;
	AC_PID pid_loiter_rate_lon;

	AC_PID pid_throttle_rate;
	AC_PID pid_throttle_accel;
	AC_PID pid_optflow_roll;
	AC_PID pid_optflow_pitch;

	APM_PI pi_loiter_lat;
	APM_PI pi_loiter_lon;
	APM_PI pi_stabilize_roll;
	APM_PI pi_stabilize_pitch;
	APM_PI pi_stabilize_yaw;
	APM_PI pi_alt_hold;

	// Note: keep initializers here in the same order as they are declared
	// above.
	Parameters() :

#if FRAME_CONFIG ==     HELI_FRAME
			        heli_servo_1(CH_1), heli_servo_2(CH_2), heli_servo_3(CH_3), heli_servo_4(CH_4),
#endif
#if FRAME_CONFIG ==     SINGLE_FRAME
			        single_servo_1(CH_1), single_servo_2(CH_2), single_servo_3(CH_3), single_servo_4(CH_4),
#endif

			        rc_1(CH_1), rc_2(CH_2), rc_3(CH_3), rc_4(CH_4), rc_5(CH_5), rc_6(CH_6), rc_7(CH_7), rc_8(CH_8),
#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
			        rc_9(CH_9),
#endif
			        rc_10(CH_10), rc_11(CH_11),
#if CONFIG_HAL_BOARD == HAL_BOARD_PX4
			        rc_12(CH_12),
#endif

			        // PID controller	initial P	        initial I		    initial D
			        //          initial imax
			        //-----------------------------------------------------------------------------------------------------
			        pid_rate_roll(RATE_ROLL_P, RATE_ROLL_I, RATE_ROLL_D, RATE_ROLL_IMAX), pid_rate_pitch(RATE_PITCH_P, RATE_PITCH_I, RATE_PITCH_D, RATE_PITCH_IMAX), pid_rate_yaw(RATE_YAW_P, RATE_YAW_I, RATE_YAW_D, RATE_YAW_IMAX),

			        pid_loiter_rate_lat(LOITER_RATE_P, LOITER_RATE_I, LOITER_RATE_D, LOITER_RATE_IMAX), pid_loiter_rate_lon(LOITER_RATE_P, LOITER_RATE_I, LOITER_RATE_D, LOITER_RATE_IMAX),

			        pid_throttle_rate(THROTTLE_RATE_P, THROTTLE_RATE_I, THROTTLE_RATE_D, THROTTLE_RATE_IMAX), pid_throttle_accel(THROTTLE_ACCEL_P, THROTTLE_ACCEL_I, THROTTLE_ACCEL_D, THROTTLE_ACCEL_IMAX), pid_optflow_roll(OPTFLOW_ROLL_P, OPTFLOW_ROLL_I,
			        OPTFLOW_ROLL_D, OPTFLOW_IMAX), pid_optflow_pitch(OPTFLOW_PITCH_P, OPTFLOW_PITCH_I, OPTFLOW_PITCH_D, OPTFLOW_IMAX),

			        // PI controller	initial P			initial I			initial
			        // imax
			        //----------------------------------------------------------------------
			        pi_loiter_lat(LOITER_P, LOITER_I, LOITER_IMAX), pi_loiter_lon(LOITER_P, LOITER_I, LOITER_IMAX),

			        pi_stabilize_roll(STABILIZE_ROLL_P, STABILIZE_ROLL_I, STABILIZE_ROLL_IMAX), pi_stabilize_pitch(STABILIZE_PITCH_P, STABILIZE_PITCH_I, STABILIZE_PITCH_IMAX), pi_stabilize_yaw(STABILIZE_YAW_P, STABILIZE_YAW_I, STABILIZE_YAW_IMAX),

			        pi_alt_hold(ALT_HOLD_P, ALT_HOLD_I, ALT_HOLD_IMAX) {
	}
};

const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;

////////////////////////////////////////////////////////////////////////////////
// Parameters
////////////////////////////////////////////////////////////////////////////////
//
// Global parameters are all contained within the 'g' class.
//
static Parameters g;
// Reference to the relay object (APM1 -> PORTL 2) (APM2 -> PORTB 7)
static AP_Relay relay;
static AP_Camera camera(&relay);
static AP_Compass_HMC5843 compass;
static AP_InertialSensor_MPU6000 ins;
static GPS *g_gps;
static GPS_Glitch gps_glitch(g_gps);
static AP_Baro_MS5611 barometer(&AP_Baro_MS5611::spi);
static AP_AHRS_DCM ahrs(ins, g_gps);
static AP_InertialNav inertial_nav(&ahrs, &barometer, g_gps, gps_glitch);
static AC_WPNav wp_nav(&inertial_nav, &ahrs, &g.pi_loiter_lat, &g.pi_loiter_lon, &g.pid_loiter_rate_lat, &g.pid_loiter_rate_lon);
static GCS_MAVLINK gcs0;
static GCS_MAVLINK gcs3;

////////////////////////////////////////////////////////////////////////////////
// 3D Location vectors
////////////////////////////////////////////////////////////////////////////////
// home location is stored when we have a good GPS lock and arm the copter
// Can be reset each the copter is re-armed
static struct Location home;
// Current location of the copter
static struct Location current_loc;
// Holds the current loaded command from the EEPROM for navigation
static struct Location command_nav_queue;
// Holds the current loaded command from the EEPROM for conditional scripts
static struct Location command_cond_queue;

// current_loc uses the baro/gps soloution for altitude rather than gps only.
// mabe one could use current_loc for lat/lon too and eliminate g_gps alltogether?
static AP_Mount camera_mount(&current_loc, g_gps, ahrs, 0);

static AP_BattMonitor battery;

// main loop scheduler
static AP_Scheduler scheduler;

// AP_Notify instance
static AP_Notify notify;

static AC_Fence fence(&inertial_nav);

////////////////////////////////////////////////////////////////////////////////
// Motor Output
////////////////////////////////////////////////////////////////////////////////
#if FRAME_CONFIG == QUAD_FRAME
#define MOTOR_CLASS AP_MotorsQuad
#elif FRAME_CONFIG == TRI_FRAME
#define MOTOR_CLASS AP_MotorsTri
#elif FRAME_CONFIG == HEXA_FRAME
#define MOTOR_CLASS AP_MotorsHexa
#elif FRAME_CONFIG == Y6_FRAME
#define MOTOR_CLASS AP_MotorsY6
#elif FRAME_CONFIG == OCTA_FRAME
#define MOTOR_CLASS AP_MotorsOcta
#elif FRAME_CONFIG == OCTA_QUAD_FRAME
#define MOTOR_CLASS AP_MotorsOctaQuad
#elif FRAME_CONFIG == HELI_FRAME
#define MOTOR_CLASS AP_MotorsHeli
#elif FRAME_CONFIG == SINGLE_FRAME
#define MOTOR_CLASS AP_MotorsSingle
#else
#error Unrecognised frame type
#endif

#if FRAME_CONFIG == HELI_FRAME  // helicopter constructor requires more arguments
static MOTOR_CLASS motors(&g.rc_1, &g.rc_2, &g.rc_3, &g.rc_4, &g.rc_7, &g.rc_8, &g.heli_servo_1, &g.heli_servo_2, &g.heli_servo_3, &g.heli_servo_4);
#elif FRAME_CONFIG == TRI_FRAME  // tri constructor requires additional rc_7 argument to allow tail servo reversing
static MOTOR_CLASS motors(&g.rc_1, &g.rc_2, &g.rc_3, &g.rc_4, &g.rc_7);
#elif FRAME_CONFIG == SINGLE_FRAME  // single constructor requires extra servos for flaps
static MOTOR_CLASS motors(&g.rc_1, &g.rc_2, &g.rc_3, &g.rc_4, &g.single_servo_1, &g.single_servo_2, &g.single_servo_3, &g.single_servo_4);
#else
static MOTOR_CLASS motors(&g.rc_1, &g.rc_2, &g.rc_3, &g.rc_4);
#endif
static RCMapper rcmap;

// Used to maintain the state of the previous control switch position
// This is set to -1 when we need to re-read the switch
static uint8_t oldSwitchPosition;

// receiver RSSI
static uint8_t receiver_rssi;

// flight modes convenience array
static AP_Int8 *flight_modes = &g.flight_mode1;

////////////////////////////////////////////////////////////////////////////////
// Failsafe
////////////////////////////////////////////////////////////////////////////////
static struct {
	uint8_t rc_override_active :1; // 0   // true if rc control are overwritten by ground station
	uint8_t radio :1; // 1   // A status flag for the radio failsafe
	uint8_t battery :1; // 2   // A status flag for the battery failsafe
	uint8_t gps :1; // 3   // A status flag for the gps failsafe
	uint8_t gcs :1; // 4   // A status flag for the ground station failsafe

	int8_t radio_counter;                  // number of iterations with throttle below throttle_fs_value

	uint32_t last_heartbeat_ms;             // the time when the last HEARTBEAT message arrived from a GCS - used for triggering gcs failsafe
} failsafe;

//Documentation of GLobals:
static union {
	struct {
		uint8_t home_is_set :1; // 0
		uint8_t simple_mode :2; // 1,2 // This is the state of simple mode : 0 = disabled ; 1 = SIMPLE ; 2 = SUPERSIMPLE

		uint8_t pre_arm_rc_check :1; // 3   // true if rc input pre-arm checks have been completed successfully
		uint8_t pre_arm_check :1; // 4   // true if all pre-arm checks (rc, accel calibration, gps lock) have been performed
		uint8_t auto_armed :1; // 5   // stops auto missions from beginning until throttle is raised
		uint8_t logging_started :1; // 6   // true if dataflash logging has started

		uint8_t do_flip :1; // 7   // Used to enable flip code
		uint8_t takeoff_complete :1; // 8
		uint8_t land_complete :1; // 9   // true if we have detected a landing

		uint8_t new_radio_frame :1; // 10      // Set true if we have new PWM data to act on from the Radio
		uint8_t CH7_flag :2; // 11,12   // ch7 aux switch : 0 is low or false, 1 is center or true, 2 is high
		uint8_t CH8_flag :2; // 13,14   // ch8 aux switch : 0 is low or false, 1 is center or true, 2 is high
		uint8_t usb_connected :1; // 15      // true if APM is powered from USB connection
		uint8_t yaw_stopped :1; // 16      // Used to manage the Yaw hold capabilities

		uint8_t disable_stab_rate_limit :1; // 17  // disables limits rate request from the stability controller

		uint8_t rc_receiver_present :1; // 18  // true if we have an rc receiver present (i.e. if we've ever received an update
	};
	uint32_t value;
} ap;

////////////////////////////////////////////////////////////////////////////////
// Radio
////////////////////////////////////////////////////////////////////////////////
// This is the state of the flight control system
// There are multiple states defined such as STABILIZE, ACRO,
static int8_t control_mode = STABILIZE;

////////////////////////////////////////////////////////////////////////////////
// CH7 and CH8 save waypoint control
////////////////////////////////////////////////////////////////////////////////
// This register tracks the current Mission Command index when writing
// a mission using Ch7 or Ch8 aux switches in flight
static int8_t aux_switch_wp_index;

////////////////////////////////////////////////////////////////////////////////
// LED output
////////////////////////////////////////////////////////////////////////////////
// Blinking indicates GPS status
static uint8_t copter_leds_GPS_blink;
// Blinking indicates battery status
static uint8_t copter_leds_motor_blink;
// Navigation confirmation blinks
static int8_t copter_leds_nav_blink;

// The current desired control scheme for Yaw
static uint8_t yaw_mode = STABILIZE_YAW;
// The current desired control scheme for roll and pitch / navigation
static uint8_t roll_pitch_mode = STABILIZE_RP;
// The current desired control scheme for altitude hold
static uint8_t throttle_mode = STABILIZE_THR;
// System Timers
// --------------
// Time in microseconds of main control loop
static uint32_t fast_loopTimer;
// Counter of main loop executions.  Used for performance monitoring and failsafe processing
static uint16_t mainLoop_count;
// Loiter timer - Records how long we have been in loiter
static uint32_t rtl_loiter_start_time;

// Used to exit the roll and pitch auto trim function
static uint8_t auto_trim_counter;

// a pin for reading the receiver RSSI voltage.
static AP_HAL::AnalogSource* rssi_analog_source;

// Input sources for battery voltage, battery current, board vcc
static AP_HAL::AnalogSource* board_vcc_analog_source;


#define GSCALAR(v, name, def) { g.v.vtype, name, Parameters::k_param_ ## v, &g.v, {def_value : def} }
#define GGROUP(v, name, class) { AP_PARAM_GROUP, name, Parameters::k_param_ ## v, &g.v, {group_info : class::var_info} }
#define GOBJECT(v, name, class) { AP_PARAM_GROUP, name, Parameters::k_param_ ## v, &v, {group_info : class::var_info} }

extern const AP_Param::Info var_info[];

bool set_roll_pitch_mode(uint8_t new_roll_pitch_mode);
bool set_yaw_mode(uint8_t new_yaw_mode);

#endif // PARAMETERS_H
