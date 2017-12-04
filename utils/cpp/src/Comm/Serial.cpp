#include "Comm/Serial.h"

// #define SHOW_LOG // use for debugging connection problems

#ifdef SHOW_LOG
  #include <iostream>
  #define LOG(X) std::cout << "[Serial " << this->portName << "] - " << X << std::endl;
#else
  #define LOG(X)
#endif

bool Comm::Serial::canRead(){
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

void Comm::Serial::send(char* data, unsigned int length){
  write(this->fileDescriptor, data, length);
}

void Comm::Serial::receive(char* data, unsigned int length){
  read(this->fileDescriptor, data, (size_t)length);
}

void Comm::Serial::connect(){
  if(this->_status == Comm::Status::Disconnected){
    this->_status = Comm::Status::Connecting;
    this->fileDescriptor = open(this->portName.c_str(), O_RDWR | O_NOCTTY);
    if(this->fileDescriptor == -1){
      LOG("Failed to open");
      this->triggerError();
    }else{

      this->configure();
      this->_status = Comm::Status::Ready;
    }
  }
}

Comm::Status Comm::Serial::status(){
  return this->_status;
}

void Comm::Serial::disconnect(){
  if(this->_status != Comm::Status::Disconnected){
    if(this->_status != Comm::Status::Error)
      this->_status = Comm::Status::Disconnected;
    close(this->fileDescriptor);
    this->fileDescriptor = 0;
  }
}

void Comm::Serial::triggerError(){
  this->_status = Comm::Status::Error;
  this->disconnect();
}

void Comm::Serial::configure(){
  // get config
  struct termios config;
  int successGettingConfig = tcgetattr(this->fileDescriptor, &config);
  if(successGettingConfig == -1){
    LOG("Failed to get config");
    this->triggerError();
    return;
  }

  //set baud rate
  int inputSpeedSupported = cfsetispeed(&config, this->speed);
  int outputSpeedSupported = cfsetospeed(&config, this->speed);
  if(inputSpeedSupported == -1 || outputSpeedSupported == -1){
    LOG("Failed to set baud rate");
    this->triggerError();
    return;
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
    LOG("Failed to set config");
    this->triggerError();
    return;
  }

  int successFlushing = tcflush(this->fileDescriptor, TCIFLUSH);
  if(successFlushing == -1){
    LOG("Failed to flush");
    this->triggerError();
    return;
  }
}
