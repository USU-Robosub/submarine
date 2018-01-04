#include <Comm/TCP/SingleClientServer.hpp>

#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Comm::TCP::SingleClientServer::SingleClientServer(unsigned int port)
  : socketFD(0){
  struct addrinfo hints, *servinfo, *p;
  struct sigaction sa;
  int yes=1;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    //return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((this->socketFD = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(this->socketFD, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(this->socketFD, p->ai_addr, p->ai_addrlen) == -1) {
      close(this->socketFD);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
  }

  if (listen(this->socketFD, BACKLOG) == -1) {
      perror("listen");
      exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
  }



  //return 0;
}

Comm::TCP::SingleClientServer::~SingleClientServer(){
  close(this->socketFD);
}

Comm::TCP::Port Comm::TCP::SingleClientServer::waitForConnection(){
  socklen_t sin_size;
  struct sockaddr_storage their_addr; // connector's address information
  int new_fd = -1;  // listen on sock_fd, new connection on new_fd
  char s[INET6_ADDRSTRLEN];

  printf("server: waiting for connections...\n");

  while(new_fd == -1) {  // main accept() loop
    sin_size = sizeof their_addr;
    new_fd = accept(this->socketFD, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
      get_in_addr((struct sockaddr *)&their_addr),
      s, sizeof s);
    printf("server: got connection from %s\n", s);

    if (!fork()) { // this is the child process
      close(this->socketFD); // child doesn't need the listener
      if (send(new_fd, "Hello, world!", 13, 0) == -1)
        perror("send");
      close(new_fd);
      exit(0);
    }
    // ----> close(new_fd);  // parent doesn't need this
  }

  return Comm::TCP::Port(new_fd);
}

/*
#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{

}
*/
