#include <Hub.hpp>
#include <settings.hpp>
#include <Arduino.h>
#include <Wire.h>

#include <Components/Motors/BlueRoboticsR1Esc.hpp>
#include <Components/Chips/ShiftRegister.hpp>
#include <Components/Sensors/MPU6050.hpp>
#include <Components/Sensors/HMC5883L.hpp>

#include <Controllers/Empty.hpp>
#include <Controllers/Echo.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Controllers/Dive.hpp>
#include <Controllers/Tank.hpp>
#include <Controllers/IMU.hpp>

typedef Components::Motors::BlueRoboticsR1Esc Motor;
typedef Components::Chips::ShiftRegister ShiftRegister;
typedef Components::Sensors::HMC5883L HMC5883L;
typedef Components::Sensors::MPU6050 MPU6050;

struct Thrusters{
  Motor* front;
  Motor* back;
  Motor* left;
  Motor* right;
};

Hub* hub;
Controller** controllers;

Thrusters thrusters;
HMC5883L* magnetometer;
MPU6050* gyroAndAccel;

void createComponents(){
  thrusters.front = new Motor({.pin=FRONT_MOTOR_PIN, .trim={MOTOR_REVERSE_MAX, MOTOR_REVERSE_MIN, MOTOR_CENTER, MOTOR_FORWARD_MIN, MOTOR_FORWARD_MAX}}),
  thrusters.back = new Motor({.pin=BACK_MOTOR_PIN, .trim={-MOTOR_REVERSE_MAX, -MOTOR_REVERSE_MIN, MOTOR_CENTER, -MOTOR_FORWARD_MIN, -MOTOR_FORWARD_MAX}}),
  thrusters.left = new Motor({.pin=LEFT_MOTOR_PIN, .trim={MOTOR_REVERSE_MAX, MOTOR_REVERSE_MIN, MOTOR_CENTER, MOTOR_FORWARD_MIN, MOTOR_FORWARD_MAX}}),
  thrusters.right = new Motor({.pin=RIGHT_MOTOR_PIN, .trim={-MOTOR_REVERSE_MAX, -MOTOR_REVERSE_MIN, MOTOR_CENTER, -MOTOR_FORWARD_MIN, -MOTOR_FORWARD_MAX}});
  Wire.begin(); // enable I2C
  magnetometer = new HMC5883L(IMU_ACCEL_MAX_SAMPLE_RATE);
  gyroAndAccel = new MPU6050(IMU_GYRO_MAX_SAMPLE_RATE);
  gyroAndAccel->setGyroFullScaleRange(GYRO_FSR_250);
  gyroAndAccel->setAccelFullScaleRange(ACCEL_FSR_2);
}

void createControllers(){
  controllers = new Controller*[CONTROLLER_COUNT];

  controllers[HUB_ECHO_PORT] = new Controllers::Echo(ECHO_RETURN);
  controllers[HUB_KILL_SWITCH_PORT] = new Controllers::KillSwitch(KILL_SWITCH_SENSE_PIN, KILL_SWITCH_HANLDER, KILL_SWITCH_DEBOUNCE_WAIT);

  controllers[HUB_DIVE_PORT] = new Controllers::Dive(thrusters.front, thrusters.back);
  controllers[HUB_TANK_PORT] = new Controllers::Tank(thrusters.left, thrusters.right);

  controllers[HUB_IMU_PORT] = new Controllers::IMU(IMU_SMAPLE_RATE, IMU_HANDLER, magnetometer, gyroAndAccel, gyroAndAccel);
}

void setupControllers(){
  static_cast<Controllers::IMU*>(controllers[HUB_IMU_PORT])->use(hub);
  static_cast<Controllers::KillSwitch*>(controllers[HUB_KILL_SWITCH_PORT])->use(hub, hub);
}

void connectToSerial(){
  hub = new Hub(controllers, CONTROLLER_COUNT);
}

void setup()
{
  createComponents();
  createControllers();
  connectToSerial();
  setupControllers();

  pinMode(13, OUTPUT);
}

void pollSerialData(){
  hub->poll();
}

void updateControllers(){
  static_cast<Controllers::IMU*>(controllers[HUB_IMU_PORT])->update();
}

unsigned long lastMillis = 0;
bool state = false;

void loop() {
  if(lastMillis + 200 < millis()){
    lastMillis = millis();
    state = !state;
    digitalWrite(13, state);
  }
  updateControllers();
  pollSerialData();
  Serial.flush();
  delay(LOOP_DELAY);
}
