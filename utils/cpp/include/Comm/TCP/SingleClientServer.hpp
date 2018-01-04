#ifndef COMM_TCP_SINGLE_CLIENT_SERVER
#define COMM_TCP_SINGLE_CLIENT_SERVER

// === Linux ====
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
// ====
#include <Comm/TCP/Port.hpp>

namespace Comm{
  namespace TCP{
    class SingleClientServer;
  }
}

class Comm::TCP::SingleClientServer{
public:
  SingleClientServer(unsigned int port);
  ~SingleClientServer();
  Comm::TCP::Port waitForConnection();

private:
  addrinfo* getAddress(const char* port);
  void createSocket(addrinfo* servinfo);

  int socketFD;
};

#endif
