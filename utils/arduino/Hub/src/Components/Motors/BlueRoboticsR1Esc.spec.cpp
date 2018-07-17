#include <catch2/catch.hpp>
#include <Components/Motors/BlueRoboticsR1Esc.hpp>

#define TEST_PIN 1

TEST_CASE("BlueRoboticsR1Esc starts disabled", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={0, 0, 0, 0, 0}});
  REQUIRE_FALSE( Servo::attached[TEST_PIN] );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == 0 );
}

TEST_CASE("BlueRoboticsR1Esc motor can be enabled and disabled", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={0, 0, 0, 0, 1}});

  motor.enable();
  REQUIRE( Servo::attached[TEST_PIN] );
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] != BR_R1_PWM_MOTOR_STOP );
  motor.disable();
  REQUIRE_FALSE( Servo::attached[TEST_PIN] );
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
}

TEST_CASE("BlueRoboticsR1Esc no trimming", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={-1, 0, 0, 0, 1}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_REVERSE );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_FORWARD );
}

TEST_CASE("BlueRoboticsR1Esc trim out reverse", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={0, 0, 0, 0, 1}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_FORWARD );
}

TEST_CASE("BlueRoboticsR1Esc trim out forward", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={-1, 0, 0, 0, 0}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_REVERSE );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
}

TEST_CASE("BlueRoboticsR1Esc switch reverse and forward with trim", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={1, 0, 0, 0, -1}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_FORWARD );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_REVERSE );
}

/*
input:    -1....0.0   0 0.0..1
range:  |__+------+___+_+----+_|
labels:  rmax   rmin  c fmin fmax
abs:    r------------s---------f
*/
TEST_CASE("BlueRoboticsR1Esc reduce top speed with trim", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={-0.5, 0, 0, 0, 0.2}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == (BR_R1_PWM_MOTOR_REVERSE - BR_R1_PWM_MOTOR_STOP) * 0.5 + BR_R1_PWM_MOTOR_STOP );
  motor.power(-0.7);
  REQUIRE( Servo::delay[TEST_PIN] == (BR_R1_PWM_MOTOR_REVERSE - BR_R1_PWM_MOTOR_STOP) * 0.5 * 0.7 + BR_R1_PWM_MOTOR_STOP );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_STOP );
  motor.power(0.3);
  REQUIRE( Servo::delay[TEST_PIN] == (BR_R1_PWM_MOTOR_FORWARD - BR_R1_PWM_MOTOR_STOP) * 0.2 * 0.3 + BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == (BR_R1_PWM_MOTOR_FORWARD - BR_R1_PWM_MOTOR_STOP) * 0.2 + BR_R1_PWM_MOTOR_STOP );
}

TEST_CASE("BlueRoboticsR1Esc change stop with center trim", "[BlueRoboticsR1Esc]"){
  Servo::$reset();
  Components::Motors::BlueRoboticsR1Esc motor({.pin=TEST_PIN, .trim={-1, 0, 0.1, 0, 1}});
  motor.enable();

  motor.power(-1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_REVERSE );
  motor.power(0);
  REQUIRE( Servo::delay[TEST_PIN] == (BR_R1_PWM_MOTOR_FORWARD - BR_R1_PWM_MOTOR_STOP) * 0.1 + BR_R1_PWM_MOTOR_STOP );
  motor.power(1);
  REQUIRE( Servo::delay[TEST_PIN] == BR_R1_PWM_MOTOR_FORWARD );
}
