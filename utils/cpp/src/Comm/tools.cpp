#include <Comm/tools.hpp>
#include <cstring>

float Comm::int32AsFloat(int32_t x){
  float temp;
  memcpy(&temp, &x, sizeof(temp));
  return temp;
}

int32_t Comm::floatAsInt32(float x){
  int32_t temp;
  memcpy(&temp, &x, sizeof(temp));
  return temp;
}
