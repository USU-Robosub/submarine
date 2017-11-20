#include "test.hpp"
#include "catch.hpp"

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    TestClass test;
    test.sayHi();
}

TEST_CASE("test1", "test2"){
  REQUIRE( false );
}
