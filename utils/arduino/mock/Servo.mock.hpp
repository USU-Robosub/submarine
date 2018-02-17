#ifndef SERVO_MOCK
#define SERVO_MOCK

#define NUM_SERVOS 10

class Servo{
public:
  void attach(int pin) {
    this->pin = pin;
    Servo::attached[this->pin] = true;
  }

  void write(int position) {
    Servo::speed[this->pin] = position;
  }

  static void reset();
  static bool attached[NUM_SERVOS];
  static int speed[NUM_SERVOS];

private:
  int pin;
};

#endif
