#include "PID/Controller.h"

double PID::Controller::correctFor(double newValue, double deltaTime){
  double error = this->setPoint - newValue;
  this->errorIntegral = Math::DiscreteCalculus::integrate(this->errorIntegral, error, deltaTime);
  double derivative = Math::DiscreteCalculus::differentiate(this->previousError, error, deltaTime);
  this->previousError = error;
  return (
    this->pGain * error +
    this->iGain * this->errorIntegral +
    this->dGain * derivative
  );
}
