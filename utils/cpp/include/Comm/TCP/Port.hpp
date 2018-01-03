#ifndef COMM_TCP_PORT
#define COMM_TCP_PORT

#include <Comm/Stream.hpp>
#include <string>
#include <map>
#include <tacopie/tacopie>

namespace Comm{
  namespace TCP{
    //std::string vectorToString(std::vector<char> raw);
    //std::queue<std::string> toQueue(std::string& data);
    class Port;
  }
}

class Comm::TCP::Port : public Comm::Stream<std::string>{
public:
  Port(std::string address, unsigned int port, unsigned int bufferSize = 1024);
  void connect();
  void host();
  void disconnect();
  void lock();
  void unlock();
  std::string poll();
  void push(std::string value);
  bool hasData();

private:
  tacopie::tcp_socket& getSocket();

  bool isServer;
  std::string address;
  unsigned int port;
  bool isRunning;
  unsigned int bufferSize;
  unsigned int timeout;
  std::shared_ptr<tacopie::tcp_server> server;
  std::shared_ptr<tacopie::tcp_client> client;
};

#endif
