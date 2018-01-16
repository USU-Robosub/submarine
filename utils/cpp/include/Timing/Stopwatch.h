#ifndef TIMING_STOPWATCH
#define TIMING_STOPWATCH

#include <chrono>

namespace Timing{
  class Stopwatch{
  public:
    double measure();
  private:
    std::chrono::high_resolution_clock::time_point lastMessureTime;
    bool firstMessure = true;
  };
}


#endif
