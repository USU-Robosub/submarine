#include "catch.hpp"
#include "Math/DiscreteCalculus.h"

TEST_CASE("the integral is a running sum", "[DiscreteCalculus]") {
  SECTION("setup #1") {
    double previousIntegral = 1;
    double newValue = 2;
    double deltaTime = 3;
    REQUIRE(
      Math::DiscreteCalculus::integrate(previousIntegral, newValue, deltaTime) == 7 // 1 + 2 * 3
    );
  }
  SECTION("setup #2") {
    double previousIntegral = 5;
    double newValue = -3;
    double deltaTime = 1;
    REQUIRE(
      Math::DiscreteCalculus::integrate(previousIntegral, newValue, deltaTime) == 2 // 5 + -3 * 1
    );
  }
}

TEST_CASE("the derivative is the slope of the last value and current value", "[DiscreteCalculus]") {
  SECTION("return 0 when delta time is 0") {
    double previousValue = 9;
    double newValue = 5;
    double deltaTime = 0;
    REQUIRE(
      Math::DiscreteCalculus::differentiate(previousValue, newValue, deltaTime) == 0 // ( 4 - 10 ) / 2
    );
  }
  SECTION("setup #1") {
    double previousValue = 10;
    double newValue = 4;
    double deltaTime = 2;
    REQUIRE(
      Math::DiscreteCalculus::differentiate(previousValue, newValue, deltaTime) == -3 // ( 4 - 10 ) / 2
    );
  }
  SECTION("setup #2") {
    double previousValue = -7;
    double newValue = -3;
    double deltaTime = 4;
    REQUIRE(
      Math::DiscreteCalculus::differentiate(previousValue, newValue, deltaTime) == 1 // ( -3 - -7 ) / 4
    );
  }
}
