#ifndef COMM_TCP_STREAM
#define COMM_TCP_STREAM

#include <Comm/Port.hpp>
#include <Comm/Stream.hpp>
#include <string>
#include <sstream>
#include <queue>

namespace Comm{
  namespace TCP{
    class Stream;
  }
}

class Comm::TCP::Stream : public Comm::Stream<std::string>{
public:
  Stream(Comm::Port<char>* port, char separator, unsigned int bufferLength = 1024);
  bool hasData();
  void push(std::string data);
  std::string poll();

private:
  Comm::Port<char>* port;
  std::string partial;
  std::queue<std::string> strings;
  char separator;
  unsigned int bufferLength;
};

#endif
