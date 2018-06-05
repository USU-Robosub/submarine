#ifndef CONTROL_CODE_HANDLER_MOCK
#define CONTROL_CODE_HANDLER_MOCK

#include <Arduino.h>

namespace Mock{
  class ControlCodeHandler{
  public:
    bool handleControlCode(){
      this->handledCode = true;
    }

    int32_t controlCode(){
      return 1;
    }

    bool handledCode = false;
  };
}

#endif
