#include <catch2/catch.hpp>
#include <Components/Chips/ShiftRegister.hpp>
#include <device/ShiftRegister.mock.hpp>

#define TEST_SC_PIN 1
#define TEST_LC_PIN 2
#define TEST_D_PIN 3
#define TEST_CONFIG {.shiftClockPin=TEST_SC_PIN, .storageClockPin=TEST_LC_PIN, .dataPin=TEST_D_PIN, .blinkDelay=0}

TEST_CASE("EightLedDisplay starts with all LEDs off", "[EightLedDisplay]"){
  Mock::Arduino::$reset();
  Mock::Device::ShiftRegister mockShiftRegister(TEST_SC_PIN, TEST_LC_PIN, TEST_D_PIN);
  Components::Chips::ShiftRegister shiftRegister(TEST_CONFIG);
  REQUIRE( Mock::Arduino::digitalPinModes[TEST_SC_PIN] == OUTPUT );
  REQUIRE( Mock::Arduino::digitalPinModes[TEST_LC_PIN] == OUTPUT );
  REQUIRE( Mock::Arduino::digitalPinModes[TEST_D_PIN] == OUTPUT );
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[0] == LOW );
  REQUIRE( mockShiftRegister.pins[1] == LOW );
  REQUIRE( mockShiftRegister.pins[2] == LOW );
  REQUIRE( mockShiftRegister.pins[3] == LOW );
  REQUIRE( mockShiftRegister.pins[4] == LOW );
  REQUIRE( mockShiftRegister.pins[5] == LOW );
  REQUIRE( mockShiftRegister.pins[6] == LOW );
  REQUIRE( mockShiftRegister.pins[7] == LOW );
}

TEST_CASE("EightLedDisplay can enable single pin", "[EightLedDisplay]"){
  Mock::Arduino::$reset();
  Mock::Device::ShiftRegister mockShiftRegister(TEST_SC_PIN, TEST_LC_PIN, TEST_D_PIN);
  Components::Chips::ShiftRegister shiftRegister(TEST_CONFIG);
  shiftRegister.pin(3, true);
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[0] == LOW );
  REQUIRE( mockShiftRegister.pins[1] == LOW );
  REQUIRE( mockShiftRegister.pins[2] == LOW );
  REQUIRE( mockShiftRegister.pins[3] == HIGH );
  REQUIRE( mockShiftRegister.pins[4] == LOW );
  REQUIRE( mockShiftRegister.pins[5] == LOW );
  REQUIRE( mockShiftRegister.pins[6] == LOW );
  REQUIRE( mockShiftRegister.pins[7] == LOW );
}

TEST_CASE("EightLedDisplay can show pattern", "[EightLedDisplay]"){
  Mock::Arduino::$reset();
  Mock::Device::ShiftRegister mockShiftRegister(TEST_SC_PIN, TEST_LC_PIN, TEST_D_PIN);
  Components::Chips::ShiftRegister shiftRegister(TEST_CONFIG);
  shiftRegister.pin(0, true);
  shiftRegister.pin(3, true);
  shiftRegister.pin(5, true);
  shiftRegister.pin(6, true);
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[0] == HIGH );
  REQUIRE( mockShiftRegister.pins[1] == LOW );
  REQUIRE( mockShiftRegister.pins[2] == LOW );
  REQUIRE( mockShiftRegister.pins[3] == HIGH );
  REQUIRE( mockShiftRegister.pins[4] == LOW );
  REQUIRE( mockShiftRegister.pins[5] == HIGH );
  REQUIRE( mockShiftRegister.pins[6] == HIGH );
  REQUIRE( mockShiftRegister.pins[7] == LOW );
}

TEST_CASE("EightLedDisplay can blink pin", "[EightLedDisplay]"){
  Mock::Arduino::$reset();
  Mock::Device::ShiftRegister mockShiftRegister(TEST_SC_PIN, TEST_LC_PIN, TEST_D_PIN);
  Components::Chips::ShiftRegister shiftRegister(TEST_CONFIG);
  shiftRegister.blinkPin(4);
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[4] == LOW );
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[4] == HIGH );
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[4] == LOW );
  shiftRegister.update();
  REQUIRE( mockShiftRegister.pins[4] == HIGH );
}
