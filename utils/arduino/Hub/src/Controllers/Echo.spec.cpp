#include <catch2/catch.hpp>
#include <Controllers/Echo.hpp>
#include <Emitter.mock.hpp>

TEST_CASE("echo sends back whatever message is received", "[Echo]"){
  int32_t buffer[3] = {0};
  Mock::Emitter emitter(buffer);
  Controllers::Echo echo(10);
  int32_t data[3] = {10, -4, 23};
  echo.execute(&emitter, data, 3);
  REQUIRE( buffer[0] == data[0] );
  REQUIRE( buffer[1] == data[1] );
  REQUIRE( buffer[2] == data[2] );
}
