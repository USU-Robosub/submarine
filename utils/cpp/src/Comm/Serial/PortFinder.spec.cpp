#include <catch.hpp>
#include <Comm/Serial/PortFinder.hpp>
#include <vector>

namespace Comm{
  namespace Serial{
    namespace PortFinder{
      extern std::vector<std::string> readDirectory(std::string name);
      extern std::vector<std::string> split(std::string s, char delim);
    }
  }
}

TEST_CASE("PortFinder will fail with exeption if it can't find the port by path", "[PortFinder]"){
  REQUIRE_THROWS( Comm::Serial::PortFinder::findByPath("will not exist") );
}

TEST_CASE("PortFinder can read a directory", "[PortFinder]"){
  std::vector<std::string> files = Comm::Serial::PortFinder::readDirectory(".");
  bool foundCurrent = false, foundParent = false;
  for(unsigned int i = 0; i < files.size() && (!foundParent || !foundCurrent); i++){
    if(files[i] == "."){
      foundCurrent = true;
    }else if(files[i] == ".."){
      foundParent = true;
    }
  }
  REQUIRE( foundCurrent );
  REQUIRE( foundParent );
}

TEST_CASE("PortFinder can't read a directory that does not exist", "[PortFinder]"){
  REQUIRE_THROWS( Comm::Serial::PortFinder::readDirectory("this directory does not exist") );
}

TEST_CASE("PortFinder can split a string", "[PortFinder]"){
  std::vector<std::string> parts = Comm::Serial::PortFinder::split("a,b,cd,e", ',');
  REQUIRE( parts.size() == 4 );
  REQUIRE( parts[0] == "a" );
  REQUIRE( parts[1] == "b" );
  REQUIRE( parts[2] == "cd" );
  REQUIRE( parts[3] == "e" );
}
