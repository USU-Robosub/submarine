#include "PID_Controller.h"

double PID_Controller::update(double processVar){
  return update(processVar, 1);
}

double PID_Controller::update(double processVar, double deltaTime){
  double error = this->setPoint - processVar;
  this->integral += error * deltaTime;
  double derivative = (error - this->lastError) / deltaTime;
  double correction = error * this->p + this->integral * this->i + derivative * this->d;
  this->lastError = error;
  return correction;
}
