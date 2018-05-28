#ifndef HUB
#define HUB

#include <Controller.hpp>
#include <Emitter.hpp>
#include <Freezable.hpp>

#include <Arduino.h>

enum class MessageState {
  CHECK, NAME, LENGTH, DATA, ALIGNING
};

struct Message {
  int32_t check;
  int32_t name;
  int32_t length;
  int32_t* data;
};

class Hub : public Emitter, public Freezable {
  public:
    Hub(Controller**, int, int maxReadsPerPoll = 100);
    void poll();
    void emit(int32_t name, int32_t* data, int32_t length);
    void freeze();
    void unfreeze();
    void test() {
      std::cout << "test hub" << std::endl;
    }
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
