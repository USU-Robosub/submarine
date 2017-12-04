#include "catch.hpp"
#include "Comm/Message.h"

TEST_CASE("data can be saved", "[Message]"){
  char data[] = { 1, 0, 0, 1, 1, 0 };
  unsigned int length = 6;
  Comm::Message message(data, length);
  for(unsigned int i = 0; i < length; i++){
    REQUIRE( data[i] == message.data[i] );
  }
}
