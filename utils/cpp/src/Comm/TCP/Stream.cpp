#include <Comm/TCP/Stream.hpp>

Comm::TCP::Stream::Stream(Comm::Port<char>* port, char separator, unsigned int bufferLength)
  : port(port),
    partial(""),
    strings(),
    separator(separator),
    bufferLength(bufferLength){}

bool Comm::TCP::Stream::hasData(){
  return ((this->strings.size() > 0) || this->port->hasData());
}

void Comm::TCP::Stream::push(std::string data){
  this->port->push((data + this->separator).c_str(), data.length() + 1);
}

#include <stdexcept>
std::string Comm::TCP::Stream::poll(){
  if(this->strings.size() == 0){
    char buffer[this->bufferLength];
    unsigned int length = this->port->poll(buffer, this->bufferLength);
    if(length == 0){
      throw std::runtime_error("attempted read after connection closed by remote");
    }
    std::stringstream ss;
    ss << this->partial;
    for(unsigned int i = 0; i < length; ++i){
      if(buffer[i] == this->separator){
        this->strings.push(ss.str());
        ss.str("");
      }else
        ss << buffer[i];
    }
    this->partial = ss.str();
  }
  if(this->strings.size() == 0){
    return "";
  }
  else{
    std::string front = strings.front();
    strings.pop();
    return front;
  }
}
