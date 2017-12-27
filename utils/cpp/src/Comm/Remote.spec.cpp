#include <catch.hpp>
#include <Comm/Remote.hpp>
#include <Comm/Hub.mock.hpp>

class TestRemote : public Comm::Remote{
public:
  TestRemote(Comm::Hub<std::string>* hub, std::string name);
  void runTest(std::vector<std::string> arguments);
  std::vector<std::string> message;
};

TEST_CASE("remote adds handlers to hub", "[Remote]"){
  Comm::Mock::Hub<std::string> hub;
  TestRemote remote(&hub, "test");

  SECTION("was added"){
    REQUIRE( hub.handlers.size() == 1 );
  }

  SECTION("was named correctly"){
    REQUIRE( hub.handlers[0].first == "test/move" );
  }

  SECTION("message was correct"){
    hub.handlers[0].second({"test"});
    REQUIRE( remote.message == std::vector<std::string>{"test"} );
  }
}

TEST_CASE("remote can emit back messages", "[Remote]"){
  Comm::Mock::Hub<std::string> hub;
  TestRemote remote(&hub, "remote");

  SECTION("was emitted"){
    REQUIRE( hub.messages.size() == 1 );
  }

  SECTION("was named correctly"){
    REQUIRE( hub.messages[0].first == "remote/echo/r" );
  }

  SECTION("message was correct"){
    REQUIRE( hub.messages[0].second == std::vector<std::string>{"1", "2", "3"} );
  }
}

TestRemote::TestRemote(Comm::Hub<std::string>* hub, std::string name)
  : Comm::Remote(hub, name){
  this->attach("move", &TestRemote::runTest, this);
  this->back("echo", {"1", "2", "3"});
}

void TestRemote::runTest(std::vector<std::string> arguments){
  this->message = arguments;
}
