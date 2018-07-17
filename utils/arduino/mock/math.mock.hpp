#ifndef MATH_MOCK
#define MATH_MOCK

#define PI 3.1415926535897932384626433832795

double map(double x, double oldMin, double oldMax, double newMin, double newMax);
double constrain(double x, double a, double b);
double abs(double value);
double tan(double value);

#ifdef MOCK_INCLUDE_SOURCE

namespace RAW_CPP{
  #include <math.h>
}

double map(double x, double oldMin, double oldMax, double newMin, double newMax){
  return (((x - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}

double constrain(double x, double a, double b){
  return (x < a ? a : (x > b ? b : x));
}

double abs(double value){
  return (value < 0 ? -value : value);
}

double tan(double value){
  return RAW_CPP::tan(value);
}

#endif
#endif
