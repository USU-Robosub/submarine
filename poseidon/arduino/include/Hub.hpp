#ifndef HUB
#define HUB

#include <Controller.hpp>
#include <Emitter.hpp>

#ifndef UNIT_TESTS
  #include <Arduino.h>
#else
  #include <Serial.mock.hpp>
#endif

enum class MessageState {
  CHECK, NAME, LENGTH, DATA, ALIGNING
};

struct Message {
  long check;
  long name;
  long length;
  long* data;
};

class Hub : public Emitter {
  public:
    Hub(Controller**, int);
    void poll();
    void emit(long name, long* data, long length);
  private:
    void serveEvent();
    long read();
    long readOneLong();
    void writeOneLong(long value);
    Controller** _controllers;
    int _numControllers;
    MessageState state;
    Message currentMessage;
    long dataLeft;
};

#endif
