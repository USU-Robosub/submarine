#include <tools.hpp>

float int32AsFloat(int32_t x){
  return *(float*)(void*)(&x);
}

int32_t floatAsInt32(float x){
  return *(int32_t*)(void*)(&x);
}
