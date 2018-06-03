#include <catch2/catch.hpp>
#include <Terminal.hpp>

SCENARIO("terminal - happy path"){
  Terminal terminal(nullptr, 100);
  REQUIRE( terminal.controlCode() == 100 );
}
