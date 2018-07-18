#ifndef MATH_MOCK
#define MATH_MOCK

#define PI 3.1415926535897932384626433832795

double map(double x, double oldMin, double oldMax, double newMin, double newMax);
double constrain(double x, double a, double b);
double abs_mock(double value);
double tan_mock(double value);
double sin_mock(double value);
double cos_mock(double value);
double sqrt_mock(double value);

#ifndef EMULATOR
  #define abs(x) abs_mock(x)
  #define tan(x) tan_mock(x)
  #define sin(x) sin_mock(x)
  #define cos(x) cos_mock(x)
  #define sqrt(x) sqrt_mock(x)
#else
  #include <math.h>
  #include <stdlib.h>
#endif

#ifdef MOCK_INCLUDE_SOURCE

#undef abs
#undef tan
#undef sin
#undef cos
#undef sqrt

#include <cmath>

double map(double x, double oldMin, double oldMax, double newMin, double newMax){
  return (((x - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}

double constrain(double x, double a, double b){
  return (x < a ? a : (x > b ? b : x));
}

double abs_mock(double value){
  return std::abs(value);
}

double tan_mock(double value){
  return std::tan(value);
}

double sin_mock(double value){
  return std::sin(value);
}

double cos_mock(double value){
  return std::cos(value);
}

double sqrt_mock(double value){
  return std::sqrt(value);
}


#endif
#endif
