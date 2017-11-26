#include "Math/DiscreteCalculus.h"

double Math::DiscreteCalculus::integrate(double previousIntegral, double newValue, double deltaTime){
  return previousIntegral + newValue * deltaTime;
}

double Math::DiscreteCalculus::differentiate(double previousValue, double newValue, double deltaTime){
  if (deltaTime == 0) return 0;
  return (newValue - previousValue) / deltaTime;
}
