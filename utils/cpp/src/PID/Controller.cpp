#include "PID/Controller.h"

PID::Controller::Controller(double pGain, double iGain, double dGain, double setPoint){
  this->pGain = pGain;
  this->iGain = iGain;
  this->dGain = dGain;
  this->setPoint = setPoint;
}

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
