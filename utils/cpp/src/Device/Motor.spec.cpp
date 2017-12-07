#include "catch.hpp"
#include "Device/Motor.h"

using namespace rxcpp;

TEST_CASE("converts power from [-1, 1] to operating range", "[Motor]"){
  observable<double> power = observable<>::create<double>([](subscriber<double> s){
    s.on_next(0.5);
    s.on_next(-1);
    s.on_completed();
  });

  observable<double> devicePower = Device::Motor::getDevicePower(
    power,
    Math::Range{.min = 0, .max = 400}
  );

  devicePower.element_at(0).subscribe([](double devicePower){
    REQUIRE(devicePower == 300);
  });
  devicePower.element_at(1).subscribe([](double devicePower){
    REQUIRE(devicePower == 0);
  });
}

TEST_CASE("keeps device power in operating range", "[Motor]"){
  observable<double> power = observable<>::create<double>([](subscriber<double> s){
    s.on_next(615.3);
    s.on_next(-1.5);
    s.on_completed();
  });

  observable<double> devicePower = Device::Motor::getDevicePower(
    power,
    Math::Range{.min = 212, .max = 352}
  );

  devicePower.element_at(0).subscribe([](double devicePower){
    REQUIRE(devicePower == 352);
  });
  devicePower.element_at(1).subscribe([](double devicePower){
    REQUIRE(devicePower == 212);
  });
}
