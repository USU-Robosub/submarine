#include "Timing/Stopwatch.h"

double Timing::Stopwatch::messure(){
  if (this->firstMessure){
    this->firstMessure = false;
    this->lastMessureTime = std::chrono::high_resolution_clock::now();
    return 0;
  }
  std::chrono::high_resolution_clock::time_point newMessureTime = std::chrono::high_resolution_clock::now();
  unsigned long deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newMessureTime - this->lastMessureTime).count();
  this->lastMessureTime = newMessureTime;
  return deltaTime * 0.001;
}
