#include <Comm/TCP/Port.hpp>

Comm::TCP::Port::Port(std::string address, unsigned int port)
  : isServer(false),
    address(address),
    port(port),
    isRunning(false),
    timeout(10) {}

Comm::TCP::Port::~Port(){
  this->disconnect();
}

bool Comm::TCP::Port::connect(){
  this->isServer = false;
  try{
    this->client = std::make_shared<tacopie::tcp_client>();
    this->client->connect(this->address, this->port, this->timeout);
  }catch(...){
    return false;
  }
  return true;
}

#include <iostream>
bool Comm::TCP::Port::host(){
  this->isServer = true;
  // try{
    this->server = std::make_shared<tacopie::tcp_server>();
    std::cout << this->address << ", " << this->port << std::endl;
    this->server->start(this->address, this->port,
      [this] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
        this->client = client;
      return true;
    });
  // }catch(...){
  //   return false;
  // }
  return true;
}

void Comm::TCP::Port::disconnect(){
  if(this->client != nullptr)
    this->client->disconnect();

  if(this->isServer && this->server != nullptr)
    this->server->stop();
}

void Comm::TCP::Port::lock(){

}

void Comm::TCP::Port::unlock(){

}

void Comm::TCP::Port::poll(char* buffer, unsigned int length){
  unsigned int bufferIndex = 0;
  while(bufferIndex < length){
    std::vector<char> data = this->getSocket().recv(length);
    for(unsigned int i = 0; i < data.size(); ++i, ++bufferIndex){
      buffer[bufferIndex] = data[i];
    }
  }
}

void Comm::TCP::Port::push(const char* buffer, unsigned int length){
  unsigned int bufferStart = 0;
  while(bufferStart < length){
    std::vector<char> data(buffer + bufferStart, buffer + length);
    std::size_t sentLength = this->getSocket().send(data, length);
    bufferStart += sentLength;
  }
}

bool Comm::TCP::Port::hasData(){

}

tacopie::tcp_socket& Comm::TCP::Port::getSocket(){
  return this->client->get_socket();
  if(this->isServer){
    return this->server->get_socket();
  }else{
    return this->client->get_socket();
  }
}
