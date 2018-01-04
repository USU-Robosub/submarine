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

void Comm::TCP::Port::lock(){
  // ?
}

void Comm::TCP::Port::unlock(){
 // ?
}

void Comm::TCP::Port::poll(char* buffer, unsigned int length){
  recv(this->socketFD, buffer, length, 0); // may not fill buffer
}

void Comm::TCP::Port::push(const char* buffer, unsigned int length){
  send(this->socketFD, buffer, length, 0); // may not send all data
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
    throw Comm::TCP::ConnectionFailure(gai_strerror(rv));
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
    throw Comm::TCP::ConnectionFailure("failed to connect");
}

// get sockaddr, IPv4 or IPv6:
void* Comm::TCP::Port::get_in_addr(sockaddr* sa)
{
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  else
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
