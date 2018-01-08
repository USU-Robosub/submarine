#ifndef COMM_TCP_PORT
#define COMM_TCP_PORT

// === Linux ====
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// ====
#include <Comm/Port.hpp>
#include <string>
#include <stdexcept>

namespace Comm{
  namespace TCP{
    class Port;
    class ConnectionFailure;
  }
}

class Comm::TCP::Port : public Comm::Port<char>{
public:
  Port(std::string address, unsigned int port);
  Port(int socketFD);
  Port(Port&& other);
  ~Port();
  void lock();
  void unlock();
  std::size_t poll(char* buffer, std::size_t length);
  void push(const char* buffer, std::size_t length);
  bool hasData();

private:
  addrinfo* getAddress(const char* address, const char* port);
  void createSocket(addrinfo* servinfo);

  int socketFD;
};

class Comm::TCP::ConnectionFailure : public std::runtime_error{
public:
  ConnectionFailure(const char* what) : std::runtime_error(what) {};
};

#endif
