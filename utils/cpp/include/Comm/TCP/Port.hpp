#ifndef COMM_TCP_PORT
#define COMM_TCP_PORT

#include <Comm/Port.hpp>
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

class Comm::TCP::Port : public Comm::Port<char>{
public:
  Port(std::string address, unsigned int port);
  bool connect();
  bool host();
  void disconnect();
  void lock();
  void unlock();
  void poll(char* buffer, unsigned int length);
  void push(const char* buffer, unsigned int length);
  bool hasData();

private:
  tacopie::tcp_socket& getSocket();

  bool isServer;
  std::string address;
  unsigned int port;
  bool isRunning;
  unsigned int timeout;
  std::shared_ptr<tacopie::tcp_server> server;
  std::shared_ptr<tacopie::tcp_client> client;
};

#endif
