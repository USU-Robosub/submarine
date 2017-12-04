#include "catch.hpp"
#include "Comm/BinaryHub.h"
#include "Comm/Stream.mock.h"
#include <thread>
#include <chrono>

TEST_CASE("can read int", "[BinaryHub]"){
  Comm::Mock::TestStream stream;
  Comm::BinaryHub hub(&stream);
  char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  stream.data = data;
  hub.lock();
  REQUIRE( hub.readInt() == 67305985 );
  REQUIRE( hub.readInt() == 16909060 );
}

TEST_CASE("can write int", "[BinaryHub]"){
  char streamData[8] = { 0 };
  Comm::Mock::TestStream stream;
  stream.data = streamData;
  Comm::BinaryHub hub(&stream);
  char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  hub.lock();
  hub.writeInt(67305985);
  hub.writeInt(16909060);
  for(unsigned int i = 0; i < 8; i++){
    REQUIRE( data[i] == stream.data[i] );
  }
}

TEST_CASE("does not read int when unlocked", "[BinaryHub]"){
  Comm::Mock::TestStream stream;
  Comm::BinaryHub hub(&stream);
  char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  stream.data = data;
  REQUIRE( hub.readInt() == 0 );
  hub.lock();
  hub.unlock();
  REQUIRE( hub.readInt() == 0 );
}

TEST_CASE("does not write int when unlocked", "[BinaryHub]"){
  char streamData[8] = { 0 };
  Comm::Mock::TestStream stream;
  stream.data = streamData;
  Comm::BinaryHub hub(&stream);
  char data[] = { 00, 00, 00, 00, 00, 00, 00, 00 };
  hub.writeInt(67305985);
  hub.lock();
  hub.unlock();
  hub.writeInt(16909060);
  for(unsigned int i = 0; i < 8; i++){
    REQUIRE( data[i] == stream.data[i] );
  }
}

void delayLockHub(Comm::BinaryHub* hub){
  hub->lock();
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  hub->writeInt(67305985);
  hub->unlock();
}

TEST_CASE("binary hub can be locked for thread safety", "[BinaryHub]"){
  char streamData[8] = { 0 };
  Comm::Mock::TestStream stream;
  stream.data = streamData;
  Comm::BinaryHub hub(&stream);
  char data[] = { 01, 02, 03, 04, 04, 03, 02, 01 };
  std::thread otherThread(delayLockHub, &hub);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  hub.lock();
  hub.writeInt(16909060);
  hub.unlock();
  for(unsigned int i = 0; i < 8; i++){
    REQUIRE( data[i] == stream.data[i] );
  }
  otherThread.join();
}
