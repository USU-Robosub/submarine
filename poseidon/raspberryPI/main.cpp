#include <iostream>

#include <Comm/Hub.hpp>
#include <Comm/TCP/Bridge.hpp>
#include <Comm/TCP/Stream.hpp>
#include <Comm/TCP/Port.hpp>
#include <Comm/TCP/SingleClientServer.hpp>
#include <Streameye.h>

#include <thread>
#include <chrono>


int main(){

  StreamEye imageStreamer;

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  
  return 0;
}
