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
    extern int analogPinStates[];
    extern int portConfig[];
    extern void(*interruptFuncs[])();
    extern int interruptTypes[];

    void $reset();
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

namespace Mock{
  namespace Arduino{
    int digitalPinModes[DIGITAL_PIN_COUNT] = { -1 };
    int digitalPinStates[DIGITAL_PIN_COUNT] = { -1 };
    int analogPinStates[ANALOG_PIN_COUNT] = { -1 };
    int portConfig[PORT_CONFIG_COUNT] = { -1 };
    void(*interruptFuncs[])() = { nullptr };
    int interruptTypes[] = { -1 };

    void $reset(){
      for(int i = 0; i < DIGITAL_PIN_COUNT; ++i){
        digitalPinModes[i] = -1;
        digitalPinStates[i] = -1;
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
  }
}

long millis(){
  return 0; // TODO
}

void delay(int ammount){
  // TODO
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
  Mock::Arduino::digitalPinStates[pin] = value;
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
