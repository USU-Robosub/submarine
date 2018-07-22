#include <Subsystem/PID.hpp>
#include <Comm/tools.hpp>
#include <iostream>

Subsystem::PID::PID(Comm::Hub<int>* arduino, int handler, Comm::Hub<std::string>* agent, std::string name)
  : Comm::Remote(agent, name),
    arduino(arduino),
    handler(handler)
    pidController(0,0,0,0){
  this->attach("setpoint", &Subsystem::PID::setpoint, this);
  this->attach("p", &Subsystem::PID::setpoint, this);
  this->attach("i", &Subsystem::PID::setpoint, this);
  this->attach("d", &Subsystem::PID::setpoint, this);
}

void Subsystem::PID::setpoint(std::vector<std::string> arguments){
  if(arguments.size() == 1){
    float amount = std::stof(arguments[0]);
    std::cout << "Set pid setpoint: " << amount << std::endl;
    this->pidController->setpoint = amount;
  }
}

void Subsystem::PID::pid(std::vector<std::string> arguments){
  if(arguments.size() == 3){
    float pGain = std::stof(arguments[0]);
    float iGain = std::stof(arguments[1]);
    float dGain = std::stof(arguments[2]);
    std::cout << "Set pid: " << pGain << "," << iGain << "," << dGain << std::endl;
    this->pidController->pGain = pGain;
    this->pidController->iGain = iGain;
    this->pidController->dGain = dGain;
  }
}

void Subsystem::PID::correct(std::vector<std::string> arguments){
  if(arguments.size() == 2){
    float newValue = std::stof(arguments[0]);
    float timeDelta = std::stof(arguments[1]);
    std::cout << "Update pid value: " << newValue << ":" << timeDelta << std::endl;
    float value = this->pidController->correctFor(newValue, timeDelta);
    this->agent->emit("pid/correction",std::vector<std::string>{std::tostring(value)})
  }
}
