#ifndef CONTROLLERS_EMPTY
#define CONTROLLERS_EMPTY

#include <Controller.hpp>

namespace Controllers{
  class Empty;
}

class Controllers::Empty : public Controller{
public:
  void execute(Emitter* hub, int32_t* data, int32_t length);
};

#endif
