#ifndef CONTROL_CODE_HANDLER
#define CONTROL_CODE_HANDLER

#include <Arduino.h>

class ControlCodeHandler{
public:
  virtual bool handleControlCode() = 0; // true - keep hub running the control code handler, false - return to normal operation
  virtual int32_t controlCode() = 0;
};

#endif
