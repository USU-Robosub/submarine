#include "Math/DiscreteCalculus.h"

namespace PID{
  class Controller{
  public:
    Controller(double pGain, double iGain, double dGain, double setPoint);
    double correctFor(double newValue, double deltaTime);
    double pGain = 0, iGain = 0, dGain = 0, setPoint = 0;
  private:
    double errorIntegral = 0, previousError = 0;
  };
}
