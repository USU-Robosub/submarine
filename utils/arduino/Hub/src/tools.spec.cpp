#include <catch2/catch.hpp>
#include <tools.hpp>

TEST_CASE("can convert float to int32 and back", "[tools]"){
  REQUIRE( int32AsFloat(floatAsInt32(1.2345f)) == 1.2345f );
  REQUIRE( int32AsFloat(floatAsInt32(-1.2345f)) == -1.2345f );
  REQUIRE( floatAsInt32(int32AsFloat(12345)) == 12345 );
  REQUIRE( floatAsInt32(int32AsFloat(-12345)) == -12345 );
}
