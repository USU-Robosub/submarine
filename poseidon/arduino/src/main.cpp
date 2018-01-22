#include <Arduino.h>

#define ENA 5
#define ENB 6
#define MOTOR_INPUT1 8
#define MOTOR_INPUT2 7
#define MOTOR_INPUT3 12
#define MOTOR_INPUT4 13
#define MOTOR_STOP    {\
    digitalWrite(MOTOR_INPUT1,LOW ); digitalWrite(MOTOR_INPUT2,LOW );\
    digitalWrite(MOTOR_INPUT3,LOW ); digitalWrite(MOTOR_INPUT4,LOW );\
}

void setup()
{
  pinMode(ENA,OUTPUT); // power level pin for left side
  pinMode(ENB,OUTPUT); // power level pin for the right side
  pinMode(MOTOR_INPUT1,OUTPUT); // direction control for right side
  pinMode(MOTOR_INPUT2,OUTPUT); // <-/
  pinMode(MOTOR_INPUT3,OUTPUT); // direction control for left side
  pinMode(MOTOR_INPUT4,OUTPUT); // <-/
}

void loop()
{
  MOTOR_STOP;
  analogWrite(ENB,90); // PWM to ENB pin
  analogWrite(ENA,90); // PWM to ENA pin
  digitalWrite(MOTOR_INPUT3,LOW ); digitalWrite(MOTOR_INPUT4,HIGH);
  delay(100);
}
