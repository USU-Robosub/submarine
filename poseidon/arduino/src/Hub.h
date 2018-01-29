#ifndef HUB
#define HUB

#include <Controller.hpp>

class Hub {
  public:
    Hub(Controller* controllers);
    void poll();
    void emit(long name, long* data, long length);
  private:
    long readOneLong();
    void writeOneLong(long value);
    Controller* _controllers;
};

#endif
