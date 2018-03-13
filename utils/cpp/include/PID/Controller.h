#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include <atomic>
#include "Math/DiscreteCalculus.h"

namespace PID{
  class Controller{
  public:
    Controller(double pGain, double iGain, double dGain, double setPoint) : pGain(pGain), iGain(iGain), dGain(dGain), setPoint(setPoint) {}
    double correctFor(double newValue, double deltaTime);
    std::atomic<double> pGain, iGain, dGain, setPoint;
  private:
    double errorIntegral = 0, previousError = 0;
  };
}

#endif
