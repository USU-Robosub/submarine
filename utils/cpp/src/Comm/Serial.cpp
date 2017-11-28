#include "Comm/Serial.h"

//#define SHOW_LOG // use for debugging connection problems

#ifdef SHOW_LOG
  #include <iostream>
  #define LOG(X) std::cout << "[Serial " << this->portName << "] - " << X << std::endl;
#else
  #define LOG(X)
#endif

bool Comm::Serial::connect(){
  this->fileDescriptor = open(this->portName.c_str(), O_RDWR | O_NOCTTY);
  if(this->fileDescriptor == -1){
    LOG("Failed to open");
    return false;
  }
  return this->configure();
}

void Comm::Serial::disconnect(){
  close(this->fileDescriptor);
  this->fileDescriptor = 0;
}

bool Comm::Serial::configure(){
  // get config
  struct termios config;
  int successGettingConfig = tcgetattr(this->fileDescriptor, &config);
  if(successGettingConfig == -1){
    LOG("Failed to get config");
    this->disconnect();
    return false;
  }

  //set baud rate
  int inputSpeedSupported = cfsetispeed(&config, this->speed);
  int outputSpeedSupported = cfsetospeed(&config, this->speed);
  if(inputSpeedSupported == -1 || outputSpeedSupported == -1){
    LOG("Failed to set baud rate");
    this->disconnect();
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
    LOG("Failed to set config");
    this->disconnect();
    return false;
  }

  int successFlushing = tcflush(this->fileDescriptor, TCIFLUSH);
  if(successFlushing == -1){
    LOG("Failed to flush");
    this->disconnect();
    return false;
  }
}