#include <catch2/catch.hpp>
#include <Terminal.hpp>
#include <string>

SCENARIO("terminal"){
  Serial.$reset();

  GIVEN("a terminal"){
    Terminal terminal(nullptr, 100);

    THEN("the control code should be set"){
      REQUIRE( terminal.controlCode() == 100 );
    }

    WHEN("handles control code for the first time"){
      REQUIRE( terminal.handleControlCode() );

      THEN("the terminal header and prompt should be shown"){
        REQUIRE( std::string((char*)Serial.writeBuffer) ==
          TERMINAL_HEADER "\n"
          TERMINAL_PROMPT );
      }
    }

    WHEN("[exit] command is sent"){
      Serial.$print("exit\n");

      THEN("the terminal should stop handling the control code after new line"){
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE_FALSE( terminal.handleControlCode() );
      }
    }

    WHEN("[clear] command is sent"){
      Serial.$print("clear\n");

      THEN("50 new lines will be sent"){
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        Serial.$clearWriteBuffer();
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( std::string((char*)Serial.writeBuffer) ==
          "\n\n\n\n\n\n\n\n\n\n"
          "\n\n\n\n\n\n\n\n\n\n"
          "\n\n\n\n\n\n\n\n\n\n"
          "\n\n\n\n\n\n\n\n\n\n"
          "\n\n\n\n\n\n\n\n\n\n"
          TERMINAL_PROMPT );
      }
    }

    WHEN("[help] command is sent"){
      Serial.$print("help\n");

      THEN("help menu will be shown"){
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        Serial.$clearWriteBuffer();
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( std::string((char*)Serial.writeBuffer) ==
          "Keys:\n"
          "  press [`] for a new prompt\n"
          "  press [ENTER] to run the entered command\n"
          "Commands:\n"
          "  clear - create some empty space above the next prompt\n"
          "  help - show available commands\n"
          "  exit - close the terminal, and return to binary input\n"
          TERMINAL_PROMPT );
      }
    }

    WHEN("typing each char is echoed back"){
      REQUIRE( terminal.handleControlCode() ); // print header
      Serial.$print("abcd");

      THEN("help menu will be shown"){
        Serial.$clearWriteBuffer();
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( terminal.handleControlCode() );
        REQUIRE( std::string((char*)Serial.writeBuffer) == "abcd" );
      }
    }
  }
}
