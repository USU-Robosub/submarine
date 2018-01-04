#include "catch.hpp"
#include "Timing/PeriodicLoop.h"
#include <chrono>
#include <thread>

void pauseFor(double seconds){
  std::this_thread::sleep_for(std::chrono::milliseconds((unsigned int)(seconds * 1000)));
}

TEST_CASE("a periodic loop can be created", "[PeriodicLoop][.timing]"){
  double minDeltaTime = 10.5;
  Timing::PeriodicLoop loop([](double deltaTime){ }, minDeltaTime);
}

TEST_CASE("calling start/stop enables/disables the loop", "[PeriodicLoop][.timing]"){
  unsigned int runCount = 0;
  Timing::PeriodicLoop loop([&runCount](double deltaTime){
    runCount++;
  }, 0.005);

  pauseFor(0.02);
  REQUIRE(runCount == 0);

  loop.start();

  pauseFor(0.02);
  REQUIRE(runCount > 0);
  unsigned int lastCount = runCount;
  pauseFor(0.02);
  REQUIRE(runCount > lastCount);
  loop.stop();
  runCount = 0;

  pauseFor(0.02);
  REQUIRE(runCount == 0);
}

TEST_CASE("loop is no ran at a shorter deltaTime", "[PeriodicLoop][.timing]"){
  unsigned int runCount = 0;
  std::chrono::high_resolution_clock::time_point t1, t2;
  double reportedDeltaTime = 0;
  double minDeltaTime = 0.01;
  Timing::PeriodicLoop loop([&runCount, &t1, &t2, &reportedDeltaTime](double deltaTime){
    runCount++;
    if(runCount == 2){
      t1 = std::chrono::high_resolution_clock::now();
    }else if(runCount == 3){
      reportedDeltaTime = deltaTime;
      t2 = std::chrono::high_resolution_clock::now();
    }
  }, minDeltaTime);
  loop.start();
  pauseFor(minDeltaTime * 5);
  loop.stop();

  double duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() * 0.001;
  REQUIRE(duration >= minDeltaTime);
  REQUIRE(abs(reportedDeltaTime - duration) < 0.002);
}
