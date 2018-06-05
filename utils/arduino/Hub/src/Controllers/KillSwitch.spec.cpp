#include <catch2/catch.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Freezable.mock.hpp>
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
    Mock::Freezable freezable;
    killSwitch.use(&freezable, nullptr);
    freezable.$makeUnfrozen();

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
        REQUIRE(freezable.$isFrozen());
        REQUIRE(freezable.$stateChanges() == 1 + 1);
      }
    }
    WHEN("the pin goes LOW then HIGH"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be active"){
        REQUIRE_FALSE(freezable.$isFrozen());
        REQUIRE(freezable.$stateChanges() == 0 + 1);
      }
    }
  }
  GIVEN("a killed kill switch"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    Mock::Freezable freezable;
    killSwitch.use(&freezable, nullptr);

    WHEN("the pin goes HIGH"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be active"){
        REQUIRE_FALSE(freezable.$isFrozen());
        REQUIRE(freezable.$stateChanges() == 1 + 1);
      }
    }
    WHEN("the pin goes HIGH then LOW"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("the emitter should be killed"){
        REQUIRE(freezable.$isFrozen());
        REQUIRE(freezable.$stateChanges() == 0 + 1);
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
    Mock::Freezable freezable;
    freezable.$makeUnfrozen();
    killSwitch.use(&freezable, nullptr);

    THEN("the emitter should be killed"){
      REQUIRE(freezable.$isFrozen());
      REQUIRE(freezable.$stateChanges() == 1);
    }
  }
  GIVEN("a LOW pin when starting"){
    Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    Mock::Freezable freezable;
    freezable.$makeUnfrozen();
    killSwitch.use(&freezable, nullptr);

    THEN("the emitter should be killed"){
      REQUIRE(freezable.$isFrozen());
      REQUIRE(freezable.$stateChanges() == 1);
    }
  }
}

SCENARIO("kill switch emits when state changes"){
  Mock::Arduino::$reset();
  MsTimer2::$reset();

  GIVEN("a killswitch with an emitter"){
    Controllers::KillSwitch killSwitch(TEST_PIN, HANDLER_ID, SECONDS(1));
    int32_t buffer[3] = {2, 2, 2};
    Mock::Freezable freezable;
    Mock::Emitter emitter(buffer);
    killSwitch.use(&freezable, &emitter);

    WHEN("the pin goes LOW"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = LOW;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("should have emitted LOW"){
        REQUIRE(buffer[0] == 0);
        REQUIRE(buffer[1] == 2);
        REQUIRE(buffer[2] == 2);
      }
    }

    WHEN("the pin goes HIGH"){
      Mock::Arduino::digitalPinStates[TEST_PIN] = HIGH;
      Mock::Arduino::interruptFuncs[TEST_PIN]();
      MsTimer2::$attachedInterrupt()();

      THEN("should have emitted HIGH"){
        REQUIRE(buffer[0] == 1);
        REQUIRE(buffer[1] == 2);
        REQUIRE(buffer[2] == 2);
      }
    }
  }

}
