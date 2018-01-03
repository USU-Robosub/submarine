#include <Comm/TCP/Port.hpp>

Comm::TCP::Port::Port(std::string address, unsigned int port, unsigned int bufferSize)
  : isServer(false),
    address(address),
    port(port),
    isRunning(false),
    bufferSize(bufferSize),
    timeout(10) {}

void Comm::TCP::Port::connect(){
  this->isServer = false;
  try{
    this->client = std::make_shared<tacopie::tcp_client>();
    this->client->connect(this->address, this->port, this->timeout);
  }catch(...){ }
}

void Comm::TCP::Port::host(){
  this->isServer = true;
  try{
    this->server = std::make_shared<tacopie::tcp_server>();
    this->server->start(this->address, this->port,
      [this] (const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
        this->client = client;
      return true;
    });
  }catch(...){ }
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

std::string Comm::TCP::Port::poll(){
  std::vector<char> data = this->getSocket().recv(this->bufferSize);
  return std::string(data.begin(), data.end());
}

void Comm::TCP::Port::push(std::string value){
  std::vector<char> data(value.begin(), value.end());
  this->getSocket().send(data, value.size());
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
