#include <Comm/Serial/Serial.hpp>

// #define SHOW_LOG // use for debugging connection problems

#ifdef SHOW_LOG
  #include <iostream>
  #define LOG(X) std::cout << "[Serial " << this->portName << "] - " << X << std::endl;
#else
  #define LOG(X)
#endif

Comm::Serial::Port::Port(std::string portName, unsigned int speed, unsigned short bufferLength)
  : portName(portName),
    speed(speed),
    connected(false),
    buffer(new unsigned char[bufferLength]),
    bufferLength(bufferLength),
    isLocked(false) {
    this->lock();
  }

bool Comm::Serial::Port::isConnected(){
  return this->connected;
}

void Comm::Serial::Port::connect(){
  if(!this->connected){
    this->fileDescriptor = open(this->portName.c_str(), O_RDWR | O_NOCTTY);
    if(this->fileDescriptor == -1){
      LOG("Failed to allocate file descriptor");
      this->connected = false;
    }else{
      this->connected = this->configure();
    }
  }
}

void Comm::Serial::Port::disconnect(){
  if(this->connected)
    close(this->fileDescriptor);
}

bool Comm::Serial::Port::hasData(){
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  fd_set readset;
  FD_ZERO(&readset);
  FD_SET(this->fileDescriptor, &readset);
  int result = select(this->fileDescriptor + 1, &readset, NULL, NULL, &timeout);
  LOG(result);
  return ( result > 0 );
}

void Comm::Serial::Port::push(unsigned char* buffer, unsigned int length){
  if(this->isLocked)
    write(this->fileDescriptor, buffer, (size_t)length);
}

void Comm::Serial::Port::poll(unsigned char* buffer, unsigned int length){
  if(this->isLocked)
    read(this->fileDescriptor, buffer, (size_t)length);
}

void Comm::Serial::Port::lock(){
  this->threadLock.lock();
  this->isLocked = true;
}

void Comm::Serial::Port::unlock(){
  this->isLocked = false;
  this->threadLock.unlock();
}

bool Comm::Serial::Port::configure(){
  // get config
  struct termios config;
  int successGettingConfig = tcgetattr(this->fileDescriptor, &config);
  if(successGettingConfig == -1){
    LOG("Failed to get config");
    return false;
  }

  //set baud rate
  int inputSpeedSupported = cfsetispeed(&config, this->speed);
  int outputSpeedSupported = cfsetospeed(&config, this->speed);
  if(inputSpeedSupported == -1 || outputSpeedSupported == -1){
    LOG("Failed to set baud rate");
    return false;
  }

  // set config bits (flags)
  // Parity = None | Stop bits = 1 | Clear bits size mask | Turn off hardware based flow control (RTS/CTS)
  config.c_cflag &= ~(PARENB|CSTOPB|CSIZE|CRTSCTS);
  // Set data bits size = 8 | Turn on the receiver | <- |
  config.c_cflag |= CS8|CREAD|CLOCAL;
  // Turn off software based flow control (XON/XOFF)
  config.c_iflag &= ~(IXON | IXOFF | IXANY);
  // Enable NON Cannonical mode
  config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  // Disable post-processing of bits
  config.c_oflag &= ~OPOST;
  // require at least one bit
  config.c_cc[VMIN] = 1;
  // instant timout for new bits (return when no new bits detected)
  config.c_cc[VTIME] = 0;

  // apply config
  int successSettingConfig = tcsetattr(this->fileDescriptor, TCSANOW, &config);
  if(successSettingConfig == -1){
    LOG("Failed to apply config");
    return false;
  }

  int successFlushing = tcflush(this->fileDescriptor, TCIFLUSH);
  if(successFlushing == -1){
    LOG("Failed to flush");
    return false;
  }
  return true;
}
