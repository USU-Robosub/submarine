#include <Comm/TCP/Port.hpp>

Comm::TCP::Port::Port(std::string address, unsigned int port)
  : isServer(false),
    address(address),
    port(port),
    isRunning(false) {}

Comm::TCP::Port::~Port(){
  //this->disconnect();
}

void Comm::TCP::Port::connect(unsigned int timeout){
  this->isServer = false;
  this->client = std::make_shared<tacopie::tcp_client>();
  try{
    this->client->connect(this->address, this->port, timeout);
  }catch(tacopie::tacopie_error e){
    throw Comm::TCP::ConnectionFailure(e.what());
  }
}

void Comm::TCP::Port::host(){
  this->isServer = true;
  this->server = std::make_shared<tacopie::tcp_server>();
  try{
    this->server->start(this->address, this->port,
      [this] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
        this->client = client;
      return true;
    });
  }catch(tacopie::tacopie_error e){
    throw Comm::TCP::ConnectionFailure(e.what());
  }
}

void Comm::TCP::Port::disconnect(){
  if(this->client != nullptr)
    this->client->disconnect(true);

  if(this->isServer && this->server != nullptr)
    this->server->stop(true);
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
