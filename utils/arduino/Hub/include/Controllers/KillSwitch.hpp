#ifndef CONTROLLERS_KILL_SWITCH
#define CONTROLLERS_KILL_SWITCH

#include <Controller.hpp>

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

private:
  void interrupt();
  bool stopped;
  long lastInterrupt;
  int numInterrupts;
  const int maxSwitchTime = 1000, waitTime = 2000, numSwitches = 2;
};

#endif
