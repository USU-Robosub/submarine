#ifndef TERMINAL
#define TERMINAL

#define TERMINAL_HEADER "-= Arduino Debug Terminal =-\n"
#define TERMINAL_PROMPT "debug@Arduino$ "

#include <ControlCodeHandler.hpp>
#include <DebugWrapper.hpp>

/*
built-in commands:
  exit - stops terminal
  help - shows help
  clear - clear screen
  ...
*/
class Terminal : public ControlCodeHandler {
public:
  Terminal(DebugWrapper* wrappers, int32_t controlCode); // the wrappers mimic the hub signals sent to a controller
  bool handleControlCode(); // true - keep hub running the control code handler, false - return to normal operation
  int32_t controlCode();

private:

};

#endif
