#ifndef MOCK_ARDUINO
#define MOCK_ARDUINO

#include <types.mock.hpp>
#include <math.mock.hpp>
#include <Serial.mock.hpp>

#define DIGITAL_PIN_COUNT 30
#define ANALOG_PIN_COUNT 30
#define PORT_CONFIG_COUNT 30
#define INTERRUPT_COUNT 30

namespace Mock{
  namespace Arduino{
    extern int digitalPinModes[];
    extern int digitalPinStates[];
    extern void(*digitalPinListeners[])(void*);
    extern void* digitalPinListenersArgs[];
    extern int analogPinStates[];
    extern int portConfig[];
    extern void(*interruptFuncs[])();
    extern int interruptTypes[];
    extern bool millisEnabled;

    void $reset();
    void $listenToDigitalPin(int pin, void(*listener)(void*), void* arg);
    void $enableMillis();
  }
}

void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
void analogWrite(int pin, int value);
int* portConfigRegister(int pin);
int digitalPinToInterrupt(int pin);
void attachInterrupt(int interrupt, void(func) (), int type);
void delay(int ammount);
long millis();
void memcpy(void* dest, void* source, int size);

#ifdef MOCK_INCLUDE_SOURCE

#include <chrono>
#include <thread>

namespace Mock{
  namespace Arduino{
    int digitalPinModes[DIGITAL_PIN_COUNT] = { -1 };
    int digitalPinStates[DIGITAL_PIN_COUNT] = { -1 };
    void(*digitalPinListeners[DIGITAL_PIN_COUNT])(void*) = { nullptr };
    void* digitalPinListenersArgs[DIGITAL_PIN_COUNT] = { nullptr };
    int analogPinStates[ANALOG_PIN_COUNT] = { -1 };
    int portConfig[PORT_CONFIG_COUNT] = { -1 };
    void(*interruptFuncs[])() = { nullptr };
    int interruptTypes[] = { -1 };
    bool millisEnabled = false;

    void $reset(){
      millisEnabled = false;
      for(int i = 0; i < DIGITAL_PIN_COUNT; ++i){
        digitalPinModes[i] = -1;
        digitalPinStates[i] = -1;
        digitalPinListeners[i] = nullptr;
        digitalPinListenersArgs[i] = nullptr;
      }
      for(int i = 0; i < ANALOG_PIN_COUNT; ++i){
        analogPinStates[i] = -1;
      }
      for(int i = 0; i < PORT_CONFIG_COUNT; ++i){
        portConfig[i] = -1;
      }
      for(int i = 0; i < INTERRUPT_COUNT; ++i){
        interruptFuncs[i] = nullptr;
        interruptTypes[i] = -1;
      }
    }

    void $listenToDigitalPin(int pin, void(*listener)(void*), void* arg){
      digitalPinListeners[pin] = listener;
      digitalPinListenersArgs[pin] = arg;
    }

    void $enableMillis(){
      millisEnabled = true;
    }
  }
}

long millis(){
  if(Mock::Arduino::millisEnabled){
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long duration = value.count();
    return duration;
  }else{
    return 0;
  }
}

void delay(int amount){
  if(Mock::Arduino::millisEnabled){
    std::this_thread::sleep_for(std::chrono::milliseconds(amount));
  }
}

void attachInterrupt(int interrupt, void(func) (), int type){
  Mock::Arduino::interruptFuncs[interrupt] = func;
  Mock::Arduino::interruptTypes[interrupt] = type;
}

int* portConfigRegister(int pin){
  return &Mock::Arduino::portConfig[pin];
}

int digitalPinToInterrupt(int pin){
  return pin;
}

void pinMode(int pin, int mode){
  Mock::Arduino::digitalPinModes[pin] = mode;
}

void digitalWrite(int pin, int value){
  bool changed = Mock::Arduino::digitalPinStates[pin] != value;
  Mock::Arduino::digitalPinStates[pin] = value;
  if(Mock::Arduino::digitalPinListeners[pin] != nullptr && changed){
    Mock::Arduino::digitalPinListeners[pin](Mock::Arduino::digitalPinListenersArgs[pin]);
  }
}

int digitalRead(int pin){
  return Mock::Arduino::digitalPinStates[pin];
}

void analogWrite(int pin, int value){
  Mock::Arduino::analogPinStates[pin] = value;
}

void memcpy(void* dest, void* source, int size){
  char* dest_temp = (char*)dest;
  char* source_temp = (char*)source;
  for(int i = 0; i < size; i++){
    dest_temp[i] = source_temp[i];
  }
}

#endif
#endif
