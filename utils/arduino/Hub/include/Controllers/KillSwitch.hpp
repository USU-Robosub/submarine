#ifndef CONTROLLERS_KILL_SWITCH
#define CONTROLLERS_KILL_SWITCH

#include <Controller.hpp>

namespace Controllers{
  class KillSwitch;
}

class Controllers::KillSwitch : public Controller{
public:
  KillSwitch(int pin, int32_t handler, Controller** controllers);
  void execute(Emitter* emitter, int32_t* data, int32_t length);
  void use(Emitter* emitter);

private:
  void interrupt();
};

#endif
