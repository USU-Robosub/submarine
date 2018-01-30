#include <Arduino.h>
#include <Hub.h>
#include <Servo.h>

#define numServos = 1
extern "C" const int servoPorts[] = { 13 };

namespace Controller {
  class Error : public Controller{
  public:

    void execute(Hub* hub, long* data, long length){
      std::cerr << "Error reading from serial." << std::endl;
    }
  };

  class Motor : public Controller{
  public:
    Motors()
    {
      for(int i = 0;i < numServos;i++)
      {
        _servo[i].attach(servoPorts[i]);
        _servo[i].writeMicroseconds(1500);
      }
    }
}

  void execute(Hub* hub, long* data, long length){
    if(length<2||data[0]>numServos||data[0]<0||data[1]>2000||data[1]<1000)
      return;
    if(data[0]>0) {
      _servo[data[1]-1]->writeMicroseconds(data[1]);
    } else {
      for(int i = 0;i < numServos;i++)
      {
        _servo[i]->writeMicroseconds(data[1]);
      }
    }
  }
private:
  Servo _servo[numServos];
};

Hub* _hub;
Controller* _controllers;

void setup()
{
  _controllers = new Controller[2];
  _controllers[0] = Controller::Error;
  _controllers[1] = Controller::Motor;
  _hub = new Hub(_controllers, 2);
}

void loop() {
  _hub->poll();
}
