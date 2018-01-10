#include <Comm/TCP/SingleClientServer.hpp>

#define BACKLOG 10 // how many pending connections queue will hold

Comm::TCP::SingleClientServer::SingleClientServer(unsigned int port)
  : socketFD(-1){
  this->createSocket(this->getAddress(std::to_string(port).c_str()));
}

Comm::TCP::SingleClientServer::~SingleClientServer(){
  if(this->socketFD >= 0)
    close(this->socketFD);
}

Comm::TCP::Port Comm::TCP::SingleClientServer::waitForConnection(){
  socklen_t sin_size;
  struct sockaddr_storage their_addr; // connector's address information
  int clientFD = -1;
  while(clientFD == -1) {
    sin_size = sizeof their_addr;
    clientFD = accept(this->socketFD, (struct sockaddr *)&their_addr, &sin_size);
    if (clientFD == -1)
      continue; // failed accept
  }
  return Comm::TCP::Port(clientFD); // wrap client with port
}

addrinfo* Comm::TCP::SingleClientServer::getAddress(const char* port){
  struct addrinfo* servinfo;
  struct addrinfo hints;
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
    throw Comm::TCP::ConnectionFailure(gai_strerror(rv));
  else
    return servinfo;
}

void Comm::TCP::SingleClientServer::createSocket(addrinfo* servinfo){
  struct addrinfo* p;
  int yes=1;
  for(p = servinfo; p != NULL; p = p->ai_next) { // bind the first one possible
    if ((this->socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      continue; // can't create socket
    if (setsockopt(this->socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      freeaddrinfo(servinfo);
      throw Comm::TCP::ConnectionFailure("failed to set socket options");
    }
    if (bind(this->socketFD, p->ai_addr, p->ai_addrlen) == -1) {
      close(this->socketFD);
      continue; // can't bind
    }
    break;
  }
  freeaddrinfo(servinfo);
  if (p == NULL)
    throw Comm::TCP::ConnectionFailure("failed to bind");
  if (listen(this->socketFD, BACKLOG) == -1)
    throw Comm::TCP::ConnectionFailure("failed to listen");
}
