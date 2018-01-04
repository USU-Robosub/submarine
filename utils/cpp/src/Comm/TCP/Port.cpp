#include <Comm/TCP/Port.hpp>

Comm::TCP::Port::Port(std::string address, unsigned int port)
  : socketFD(-1){
  this->createSocket(this->getAddress(address.c_str(), std::to_string(port).c_str()));
  //
  // // ==== done ====
  //
  // char buf[MAXDATASIZE];
  // int numbytes;
  // if ((numbytes = recv(this->socketFD, buf, MAXDATASIZE-1, 0)) == -1) {
  //   perror("recv");
  //   exit(1);
  // }
  //
  // buf[numbytes] = '\0';
  //
  // printf("client: received '%s'\n",buf);
}

Comm::TCP::Port::Port(int socketFD)
  : socketFD(socketFD){}

Comm::TCP::Port::Port(Comm::TCP::Port&& other)
  : socketFD(-1){
  this->socketFD = other.socketFD;
  other.socketFD = -1;
}

#include <iostream>

Comm::TCP::Port::~Port(){
  if(this->socketFD >= 0)
    close(this->socketFD);
}

void Comm::TCP::Port::lock(){

}

void Comm::TCP::Port::unlock(){

}

void Comm::TCP::Port::poll(char* buffer, unsigned int length){
  recv(this->socketFD, buffer, length, 0);
}

void Comm::TCP::Port::push(const char* buffer, unsigned int length){
  send(this->socketFD, buffer, length, 0);
}

bool Comm::TCP::Port::hasData(){
  int n, rv;
  fd_set readfds;
  struct timeval tv;
  char buf1[256], buf2[256];

  // clear the set ahead of time
  FD_ZERO(&readfds);

  // add our descriptors to the set
  FD_SET(this->socketFD, &readfds);
  //FD_SET(s2, &readfds);

  // since we got s2 second, it's the "greater", so we use that for
  // the n param in select()
  n = this->socketFD + 1;

  // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  rv = select(n, &readfds, NULL, NULL, &tv);

  if (rv == -1) {
    perror("select"); // error occurred in select()
  } else if (rv == 0) {
    printf("Timeout occurred!  No data after 10.5 seconds.\n");
  } else {
    // one or both of the descriptors have data
    if (FD_ISSET(this->socketFD, &readfds)) {
      return true;
    }
  }
  return false;
}

addrinfo* Comm::TCP::Port::getAddress(const char* address, const char* port){
  struct addrinfo* servinfo;
  int rv;
  struct addrinfo hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
    throw Comm::TCP::ConnectionFailure(gai_strerror(rv));
  }
  return servinfo;
}

void Comm::TCP::Port::createSocket(addrinfo* servinfo){
  struct addrinfo* p;
  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((this->socketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      // can't create socket
      continue;
    }
    if (connect(this->socketFD, p->ai_addr, p->ai_addrlen) == -1) {
      // can't connect
      close(this->socketFD);
      continue;
    }
    break;
  }
  if (p == NULL) {
    throw Comm::TCP::ConnectionFailure("failed to connect");
  }
  freeaddrinfo(servinfo); // all done with this structure
}

// get sockaddr, IPv4 or IPv6:
void* Comm::TCP::Port::get_in_addr(sockaddr* sa)
{
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  else
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
