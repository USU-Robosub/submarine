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
    PeriodicLoop(std::function<void(double)> callback, double minDeltaTime) 
	    : callback(callback)
	    , minDeltaTime(std::chrono::duration<unsigned long long,std::nano>((unsigned long long)(minDeltaTime*1e9)))
	    , isEnabled(false) {}
    void start();
    void stop();
    void join();
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    int loops;
    Timing::Stopwatch loopStopwatch;
    std::atomic<bool> isEnabled;
    void threadFunction();
    std::thread* thread;
    std::function<void(double)> callback;
    std::chrono::nanoseconds minDeltaTime;
  };
}

#endif
