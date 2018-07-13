#include <tools.hpp>
#include <Arduino.h>

float int32AsFloat(int32_t x){
  float temp;
  memcpy(&temp, &x, sizeof(temp));
  return temp;
  //return *(float*)(void*)(&x);
}

int32_t floatAsInt32(float x){
  int32_t temp;
  memcpy(&temp, &x, sizeof(temp));
  return temp;
  //return *(int32_t*)(void*)(&x);
}
