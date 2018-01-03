#ifndef COMM_TCP_PORT
#define COMM_TCP_PORT

#include <Comm/Port.hpp>
#include <string>
#include <map>
#include <tacopie/tacopie>

namespace Comm{
  namespace TCP{
    class Port;
    class ConnectionFailure;
  }
}

class Comm::TCP::Port : public Comm::Port<char>{
public:
  Port(std::string address, unsigned int port);
  ~Port();
  void connect(unsigned int timeout = 10);
  void host();
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
  std::shared_ptr<tacopie::tcp_server> server;
  std::shared_ptr<tacopie::tcp_client> client;
};

class Comm::TCP::ConnectionFailure : public std::runtime_error{
public:
  ConnectionFailure(const char* what) : std::runtime_error(what) {};
};

#endif
