#include "Timing/PeriodicLoop.h"
#include <iostream>

void Timing::PeriodicLoop::start(){
  this->thread = new std::thread(&Timing::PeriodicLoop::threadFunction, this);
  this->isEnabled = true;
}

void Timing::PeriodicLoop::threadFunction(){
  while(this->isEnabled){
    double deltaTime = this->loopStopwatch.messure();
    this->codeStopwatch.messure();
    this->callback(deltaTime);
    double timeLeft = this->minDeltaTime - this->codeStopwatch.messure();
    std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(timeLeft * 1000)));
  }
}

void Timing::PeriodicLoop::stop(){
  this->isEnabled = false;
  this->thread->join();
  delete this->thread;
}