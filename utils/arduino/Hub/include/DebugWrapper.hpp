#ifndef DEBUG_WRAPPER
#define DEBUG_WRAPPER

#include <Arduino.h>

struct CommandList {
  String* commands; // thing to type in to activate the command
  String* helps; // help text shown by the help command
  unsigned int count;
};

class DebugWrapper {
public:
  virtual CommandList commands() = 0;
  virtual bool run() = 0; // true - keep running the command, false - continue with terminal
};

#endif
