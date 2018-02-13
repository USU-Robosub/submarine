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
  int32_t check;
  int32_t name;
  int32_t length;
  int32_t* data;
};

class Hub : public Emitter {
  public:
    Hub(Controller**, int, int maxReadsPerPoll = 100);
    void poll();
    void emit(int32_t name, int32_t* data, int32_t length);
  private:
    void serveEvent();
    int32_t read();
    int32_t readInt();
    void writeInt(int32_t value);
    Controller** _controllers;
    int _numControllers;
    MessageState state;
    Message currentMessage;
    int32_t dataLeft;
    int maxReadsPerPoll;
};

#endif
