#include <thread>
#include <chrono>
#include "catch.hpp"
#include "Timing/Stopwatch.h"

TEST_CASE("a stopwatch starts with 0s deltaTime", "[Stopwatch][.timing]"){
  Timing::Stopwatch stopwatch;
  REQUIRE( stopwatch.messure() == 0 );
}

TEST_CASE("a stopwatch messures a delay correctly", "[Stopwatch][.timing]"){
  Timing::Stopwatch stopwatch;
  stopwatch.messure();
  stopwatch.messure();
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  double deltaTime = stopwatch.messure();
  std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
  unsigned long pauseTimeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  double pauseTime = pauseTimeMilliseconds * 0.001;
  REQUIRE( abs(deltaTime - pauseTime) <= 0.002 );
}
