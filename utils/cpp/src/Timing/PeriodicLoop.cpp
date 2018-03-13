#include "Timing/PeriodicLoop.h"
#include <iostream>

void Timing::PeriodicLoop::start(){
  this->thread = new std::thread(&Timing::PeriodicLoop::threadFunction, this);
  this->isEnabled = true;
  startTime = std::chrono::high_resolution_clock::now();
  loops = 0;
  this->loopStopwatch.measure();
}

void Timing::PeriodicLoop::threadFunction(){
  while(this->isEnabled){
    loops++;
    double deltaTime = this->loopStopwatch.measure();
    this->callback(deltaTime);
    std::chrono::nanoseconds sleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime+(minDeltaTime*loops)-std::chrono::high_resolution_clock::now());
    // if(sleepTime > 0)
    // {//Maybe add an option for skipping frames if overrun on time
      std::this_thread::sleep_for(sleepTime);
    //}
  }
}

void Timing::PeriodicLoop::stop(){
  this->isEnabled = false;
}

void Timing::PeriodicLoop::join(){
  this->thread->join();
  delete this->thread;
}
