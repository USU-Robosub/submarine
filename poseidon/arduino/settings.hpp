#ifndef ARDUINO_SETTINGS
#define ARDUINO_SETTINGS

#define KILL_PIN 3
#define ECHO_RETURN 42

// motor pins
#define FRONT_MOTOR_PIN 5
#define BACK_MOTOR_PIN 4
#define LEFT_MOTOR_PIN 3
#define RIGHT_MOTOR_PIN 6

// motor trims
#define MOTOR_REVERSE_MAX -0.9
#define MOTOR_REVERSE_MIN -0.1
#define MOTOR_CENTER 0
#define MOTOR_FORWARD_MIN 0.1
#define MOTOR_FORWARD_MAX 0.9
#define MOTOR_DEFAULT_TRIM {MOTOR_REVERSE_MAX, MOTOR_REVERSE_MIN, MOTOR_CENTER, MOTOR_FORWARD_MIN, MOTOR_FORWARD_MAX}

// IMU
#define IMU_ACCEL_MAX_SAMPLE_RATE 10 // ms
#define IMU_GYRO_MAX_SAMPLE_RATE 10 // ms
#define IMU_SMAPLE_RATE 100 // ms
#define IMU_HANDLER 10

// controllers
#define HUB_ECHO_PORT 0
#define HUB_KILL_SWITCH_PORT 1
#define HUB_DIVE_PORT 2
#define HUB_TANK_PORT 3
#define HUB_IMU_PORT 4

#define CONTROLLER_COUNT 5

#define LOOP_DELAY 1

#endif
