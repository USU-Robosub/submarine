#include <Comm/TCP/Port.hpp>

Comm::TCP::Port::Port(std::string address, unsigned int port)
  : socketFD(-1){
  this->createSocket(this->getAddress(address.c_str(), std::to_string(port).c_str()));
}

Comm::TCP::Port::Port(int socketFD)
  : socketFD(socketFD){}

Comm::TCP::Port::Port(Comm::TCP::Port&& other)
  : socketFD(-1){
  this->socketFD = other.socketFD;
  other.socketFD = -1;
}

Comm::TCP::Port::~Port(){
  if(this->socketFD >= 0)
    close(this->socketFD);
}

std::size_t Comm::TCP::Port::poll(char* buffer, std::size_t length){
  return recv(this->socketFD, buffer, length, 0); // should check if errored
}

void Comm::TCP::Port::push(const char* buffer, std::size_t length){
  send(this->socketFD, buffer, length, 0); // may not send all data | should check if errored
}

bool Comm::TCP::Port::hasData(){
  fd_set readfds;
  struct timeval tv{.tv_sec = 0, .tv_usec = 0};
  FD_ZERO(&readfds);
  FD_SET(this->socketFD, &readfds);
  int rv = select(this->socketFD + 1, &readfds, NULL, NULL, &tv);
  if (rv != -1 && rv != 0 && FD_ISSET(this->socketFD, &readfds))
    return true; // select shows data available
  else
    return false;
}

addrinfo* Comm::TCP::Port::getAddress(const char* address, const char* port){
  struct addrinfo* servinfo;
  struct addrinfo hints;
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0)
    throw Comm::ConnectionFailure(gai_strerror(rv));
  else
    return servinfo;
}

void Comm::TCP::Port::createSocket(addrinfo* servinfo){
  struct addrinfo* p;
  for(p = servinfo; p != NULL; p = p->ai_next) { // connect the first one possible
    if ((this->socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      continue; // can't create socket
    if (connect(this->socketFD, p->ai_addr, p->ai_addrlen) == -1) {
      close(this->socketFD);
      continue; // can't connect
    }
    break;
  }
  freeaddrinfo(servinfo);
  if (p == NULL)
    throw Comm::ConnectionFailure("failed to connect");
}
