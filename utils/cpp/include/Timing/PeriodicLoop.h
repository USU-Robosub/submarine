#ifndef TIMING_PERIODIC_LOOP
#define TIMING_PERIODIC_LOOP

#include <functional>
#include <thread>
#include <atomic>
#include <chrono>
#include "Timing/Stopwatch.h"

namespace Timing{
  class PeriodicLoop{
  public:
    PeriodicLoop(std::function<void(double)> callback, double minDeltaTime) : callback(callback), minDeltaTime(minDeltaTime), isEnabled(false) {}
    void start();
    void stop();
  private:
    Timing::Stopwatch loopStopwatch;
    Timing::Stopwatch codeStopwatch;
    std::atomic<bool> isEnabled;
    void threadFunction();
    std::thread* thread;
    std::function<void(double)> callback;
    double minDeltaTime;
  };
}

#endif
