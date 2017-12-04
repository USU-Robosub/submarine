#include "catch.hpp"
#include "Comm/MessageBuilder.h"
#include "Comm/Stream.mock.h"

TEST_CASE("message builder can read a simple message", "[MessageBuilder]"){
  Comm::Mock::TestStream stream;
  Comm::MessageBuilder builder(&stream);

  char data[] = { 5, 0, 0, 1, 1, 0 };
  unsigned int length = 5;
  stream.data = data;

  Comm::Message message = builder.read();

  REQUIRE( message.length == length );

  for(unsigned int i = 0; i < length; i++){
    REQUIRE( data[i + 1] == message.data[i] );
  }
}

TEST_CASE("message builder can read a long message", "[MessageBuilder]"){
  Comm::Mock::TestStream stream;
  Comm::MessageBuilder builder(&stream);

  char data[] = { 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
  unsigned int length = 20;
  stream.data = data;

  Comm::Message message = builder.read();

  REQUIRE( message.length == length );

  for(unsigned int i = 0; i < length; i++){
    REQUIRE( data[i + 1] == message.data[i] );
  }
}
