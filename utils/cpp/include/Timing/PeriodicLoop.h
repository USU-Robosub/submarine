#include <functional>
#include <thread>
#include <chrono>
#include "Timing/Stopwatch.h"

namespace Timing{
  class PeriodicLoop{
  public:
    PeriodicLoop(std::function<void(double)> callback, double minDeltaTime) : callback(callback), minDeltaTime(minDeltaTime) {}
    void start();
    void stop();
  private:
    Timing::Stopwatch loopStopwatch;
    Timing::Stopwatch codeStopwatch;
    bool isEnabled = false;
    void threadFunction();
    std::thread* thread;
    std::function<void(double)> callback;
    double minDeltaTime;
  };
}
