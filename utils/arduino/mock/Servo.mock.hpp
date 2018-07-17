#ifndef SERVO_MOCK
#define SERVO_MOCK

#define NUM_SERVOS 10

class Servo{
public:
  void attach(int pin) {
    this->pin = pin;
    Servo::attached[this->pin] = true;
  }

  void detach() {
    // TODO not implemented
  }

  void write(int position) {
    Servo::speed[this->pin] = position;
  }

  static void $reset(){
    for(int i = 0; i < NUM_SERVOS; ++i){
      Servo::attached[i] = false;
      Servo::speed[i] = 90;
    }
  }

  static bool attached[NUM_SERVOS];
  static int speed[NUM_SERVOS];

private:
  int pin;
};

#ifdef MOCK_INCLUDE_SOURCE

bool Servo::attached[] = {false};
int Servo::speed[NUM_SERVOS] = {90};

#endif
#endif
