#include "PID_Controller.h"

double PID_Controller::update(double processVar){
  double error = this->setPoint - processVar;
  this->integral += error;
  double derivative = error - this->lastError;
  double correction = error * this->p + this->integral * this->i + derivative * this->d;
  this->lastError = error;
  return correction;
}
