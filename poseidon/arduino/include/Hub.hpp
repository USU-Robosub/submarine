#ifndef HUB
#define HUB

#include <Controller.hpp>
#include <Emitter.hpp>

#ifndef UNIT_TESTS
  #include <Arduino.h>
#else
  #include <Serial.mock.hpp>
#endif

class Hub : public Emitter {
  public:
    Hub(Controller**, int);
    void poll();
    void emit(long name, long* data, long length);
  private:
    long readOneLong();
    void writeOneLong(long value);
    Controller** _controllers;
    int _numControllers;
};

#endif
