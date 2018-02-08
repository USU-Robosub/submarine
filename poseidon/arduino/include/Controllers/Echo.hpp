#ifndef CONTROLLERS_ECHO
#define CONTROLLERS_ECHO

#include <Controller.hpp>

namespace Controllers{
  class Echo;
}

class Controllers::Echo : public Controller{
public:
  Echo(int32_t returnName);
  void execute(Emitter* hub, int32_t* data, int32_t length);

private:
  int32_t returnName;
};

#endif
