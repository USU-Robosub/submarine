#include <catch2/catch.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Emitter.mock.hpp>

#define TEST_PIN 10
#define HANDLER_ID 13

SCENARIO("kill switches can be constructed"){
  Mock::Arduino::$reset();
  MsTimer2::$reset();

  GIVEN("a kill switch"){
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));

    THEN("the pin should be set to input and pulled low"){
      REQUIRE(Mock::Arduino::digitalPinModes[TEST_PIN] == INPUT);
      REQUIRE(Mock::Arduino::portConfig[TEST_PIN] == 0); // pull pin low (input) on teensy, they are pulled high by default
    }
    THEN("timer2 should be setup for debounce"){
      REQUIRE(MsTimer2::$delay() == MILLI_SECONDS(1000));
      REQUIRE(MsTimer2::$attachedInterrupt() == Controllers::KillSwitch::debounce);
      REQUIRE(MsTimer2::$isStopped());
    }
    THEN("a on change interupt should be attached to the pin"){
      REQUIRE(Mock::Arduino::interruptFuncs[TEST_PIN] == Controllers::KillSwitch::interrupt);
      REQUIRE(Mock::Arduino::interruptTypes[TEST_PIN] == CHANGE);
    }
  }
}

SCENARIO("kill switches trigger when the pin's state changes"){
  Mock::Arduino::$reset();
  MsTimer2::$reset();

  GIVEN("an active kill switch"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    int32_t buffer[3] = {0};
    Mock::Emitter emitter(buffer);
    killSwitch.use(&emitter);
    emitter.$makeActive();

    WHEN("the pin goes LOW"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      THEN("timer2 should be started before interrupt"){
        REQUIRE_FALSE(MsTimer2::$isStopped());
      }
      MsTimer2::$attachedInterrupt()();
      THEN("timer2 should be stopped after interrupt"){
        REQUIRE(MsTimer2::$isStopped());
      }

      THEN("the emitter should be killed"){
        REQUIRE(emitter.$isKilled());
        REQUIRE(emitter.$stateChanges() == 1 + 1);
      }
    }
    WHEN("the pin goes LOW then HIGH"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be active"){
        REQUIRE_FALSE(emitter.$isKilled());
        REQUIRE(emitter.$stateChanges() == 0 + 1);
      }
    }
  }
  GIVEN("a killed kill switch"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    int32_t buffer[3] = {0};
    Mock::Emitter emitter(buffer);
    killSwitch.use(&emitter);

    WHEN("the pin goes HIGH"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be active"){
        REQUIRE_FALSE(emitter.$isKilled());
        REQUIRE(emitter.$stateChanges() == 1 + 1);
      }
    }
    WHEN("the pin goes HIGH then LOW"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be killed"){
        REQUIRE(emitter.$isKilled());
        REQUIRE(emitter.$stateChanges() == 0 + 1);
      }
    }
  }
}

SCENARIO("kill switches allways starts killed"){
  Mock::Arduino::$reset();
  MsTimer2::$reset();

  GIVEN("a HIGH pin when starting"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    int32_t buffer[3] = {0};
    Mock::Emitter emitter(buffer);
    emitter.$makeActive();
    killSwitch.use(&emitter);

    THEN("the emitter should be killed"){
      REQUIRE(emitter.$isKilled());
      REQUIRE(emitter.$stateChanges() == 1);
    }
  }
  GIVEN("a LOW pin when starting"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    int32_t buffer[3] = {0};
    Mock::Emitter emitter(buffer);
    emitter.$makeActive();
    killSwitch.use(&emitter);

    THEN("the emitter should be killed"){
      REQUIRE(emitter.$isKilled());
      REQUIRE(emitter.$stateChanges() == 1);
    }
  }
}

// TEST_CASE("kill switch, constructor", "[killSwitch]"){
//   Mock::Arduino::$reset();
//   Controllers::KillSwitch killSwitch(TEST_PIN, 13, SECONDS(1));
//
//   REQUIRE(Mock::Arduino::digitalPinModes[TEST_PIN] == 0); // make pin input on all other devices
//   REQUIRE(Mock::Arduino::portConfig[TEST_PIN] == 0); // pull pin low (input) on teensy, they are pulled high by default
//
//   REQUIRE(Timer1.$delay() == 1000000);
//   REQUIRE(Timer1.$attachedInterrupt() == Controllers::KillSwitch::debounce);
//   REQUIRE(Timer1.$isStopped());
//
//   REQUIRE(Controllers::KillSwitch::killSwitch == &killSwitch);
//
//   REQUIRE(Mock::Arduino::interruptFuncs[TEST_PIN] == Controllers::KillSwitch::interrupt);
// }
//
// TEST_CASE("trigger interupt on + off", "[killSwitch]"){
//   Mock::Arduino::$reset();
//   Controllers::KillSwitch killSwitch(TEST_PIN, 13, SECONDS(1));
//   int32_t buffer[3] = {0};
//   Mock::Emitter emitter(buffer);
//   killSwitch.use(&emitter);
//   REQUIRE_FALSE(emitter.$isStarted());
//
//   Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
//   Mock::Arduino::interruptFuncs[TEST_PIN](); // pin changed to high
//   Timer1.$attachedInterrupt()(); // timer expired
//   REQUIRE(emitter.$isStarted());
//   REQUIRE(emitter.$stateChanges() == 1);
//
//   Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
//   Mock::Arduino::interruptFuncs[TEST_PIN]();
//   Timer1.$attachedInterrupt()();
//   REQUIRE_FALSE(emitter.$isStarted());
//   REQUIRE(emitter.$stateChanges() == 2);
// }
//
// TEST_CASE("trigger interupt debounce", "[killSwitch]"){
//   Mock::Arduino::$reset();
//   Controllers::KillSwitch killSwitch(TEST_PIN, 13, SECONDS(1));
//   int32_t buffer[3] = {0};
//   Mock::Emitter emitter(buffer);
//   killSwitch.use(&emitter);
//
//   REQUIRE_FALSE(emitter.$isStarted());
//   Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
//   Mock::Arduino::interruptFuncs[TEST_PIN]();
//   Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
//   Timer1.$attachedInterrupt()();
//   REQUIRE_FALSE(emitter.$isStarted());
//   REQUIRE(emitter.$stateChanges() == 0);
//
//   Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
//   Mock::Arduino::interruptFuncs[TEST_PIN]();
//   Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
//   Timer1.$attachedInterrupt()();
//   REQUIRE_FALSE(emitter.$isStarted());
//   REQUIRE(emitter.$stateChanges() == 0);
// }
//
// TEST_CASE("trigger interupt double click", "[killSwitch]"){
//   Mock::Arduino::$reset();
//   Controllers::KillSwitch killSwitch(TEST_PIN, 13, SECONDS(1));
//   int32_t buffer[3] = {0};
//   Mock::Emitter emitter(buffer);
//   killSwitch.use(&emitter);
//
//   REQUIRE_FALSE(emitter.$isStarted());
//   Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
//   Mock::Arduino::interruptFuncs[TEST_PIN]();
//   Timer1.$attachedInterrupt()();
//   Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
//   Mock::Arduino::interruptFuncs[TEST_PIN]();
//   Timer1.$attachedInterrupt()();
//   REQUIRE(emitter.$isStarted());
//   REQUIRE(emitter.$stateChanges() == 0);
// }
