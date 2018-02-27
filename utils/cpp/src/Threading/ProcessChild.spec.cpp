#include <catch.hpp>
#include <Threading/ProcessChild.hpp>

TEST_CASE("can read data from a child process", "[ProcessChild]"){
  Threading::ProcessChild echo("echo", {"hello", "world", "!"});
  unsigned char buffer[13] = {0};
  REQUIRE( echo.read(buffer, 13) == 13 );
  REQUIRE( std::string((char*)buffer, 13) == "hello world !" );
}

TEST_CASE("can send data to a child process", "[ProcessChild]"){
  Threading::ProcessChild cat("cat", {});
  REQUIRE( cat.write((unsigned char*)"this is a test", 14) == 14);
  unsigned char buffer[14] = {0};
  REQUIRE( cat.read(buffer, 14) == 14 );
  REQUIRE( std::string((char*)buffer, 14) == "this is a test" );
}

TEST_CASE("child process no data", "[ProcessChild]"){
  Threading::ProcessChild echo("echo", {"1"});
  unsigned char buffer[5] = {0};
  REQUIRE( echo.read(buffer, 5) == 2 );
  REQUIRE( std::string((char*)buffer, 2) == "1\n" );
}
