#ifndef MATH_MOCK
#define MATH_MOCK

double map(double x, double oldMin, double oldMax, double newMin, double newMax);
int constrain(int x, int a, int b);
double abs(double value);

#ifdef MOCK_INCLUDE_SOURCE

double map(double x, double oldMin, double oldMax, double newMin, double newMax){
  return (((x - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}

int constrain(int x, int a, int b){
  return (x < a ? a : (x > b ? b : x));
}

double abs(double value){
  return (value < 0 ? -value : value);
}

#endif
#endif
