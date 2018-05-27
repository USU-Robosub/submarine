#ifndef CONTROLLERS_KILL_SWITCH
#define CONTROLLERS_KILL_SWITCH

#include <Controller.hpp>
#include <MsTimer2.h>

namespace Controllers{
  class KillSwitch;
}

#define SECONDS(x) x * 1000
#define MILLI_SECONDS(x) x

class Controllers::KillSwitch : public Controller{
public:
  KillSwitch(int pin, int32_t handler, int debounceDelay);
  void execute(Emitter* emitter, int32_t* data, int32_t length);
  void use(Emitter* emitter);
  void kill(){};
  void restart(){};

  static void interrupt();
  static void debounce();
  static Controllers::KillSwitch* killSwitch;
  static bool checkingDebounce;
  static bool startSate;
protected:
  int32_t handler;
  //bool stopped;
  int pin;
  //unsigned long lastInterrupt;
  //int numInterrupts;
  //const unsigned long maxSwitchTime = 1000, waitTime = 2000;
  //const int numSwitches = 2;
  Emitter* emitter;
};

#endif
