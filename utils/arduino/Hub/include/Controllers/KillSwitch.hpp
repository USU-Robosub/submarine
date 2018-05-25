#ifndef CONTROLLERS_KILL_SWITCH
#define CONTROLLERS_KILL_SWITCH

#include <Controller.hpp>
#include <TimerOne.h>

namespace Controllers{
  class KillSwitch;
}

class Controllers::KillSwitch : public Controller{
public:
  KillSwitch(int pin, int32_t handler);
  void execute(Emitter* emitter, int32_t* data, int32_t length);
  void use(Emitter* emitter);
  void kill(){};
  void restart(){};

protected:
  int32_t handler;
  static Controllers::KillSwitch* killSwitch;
  static void interrupt();
  bool stopped;
  unsigned long lastInterrupt;
  int numInterrupts;
  const unsigned long maxSwitchTime = 1000, waitTime = 2000;
  const int numSwitches = 2;
  Emitter* emitter;
};

#endif
