#include <Comm/Serial/Port.hpp>
#include <fstream>
#include <iostream>
//#define SHOW_LOG // use for debugging connection problems
//#define SHOW_BUFFER_LOG

#ifdef SHOW_LOG
  #include <iostream>
  #define LOG(X) std::cout << "[Serial " << this->portName << "] - " << X << std::endl;
#else
  #define LOG(X)
#endif

#ifdef SHOW_BUFFER_LOG
  #include <iomanip>
  #define LOG_BUFFER(Z, X, Y) std::cout << "[Serial " << this->portName << "] - " << Z << ": "; for(unsigned int i = 0; i < Y; ++i) { std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) (X[i]) << ' '; } std::cout << std::endl;
#else
  #define LOG_BUFFER(Z, X, Y)
#endif

Comm::Serial::Port::Port(std::string portName, unsigned int speed)
  : portName(portName),
    speed(speed) {
    this->fileDescriptor = open(this->portName.c_str(), O_RDWR | O_NOCTTY);
    if(this->fileDescriptor == -1){
      throw Comm::ConnectionFailure("Failed to connect to serial port. Failed to allocate file descriptor.");
    }else{
      this->configure();
    }
  }

Comm::Serial::Port::~Port(){
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
  return ( result > 0 );
}

void Comm::Serial::Port::push(const unsigned char* buffer, std::size_t length){
  unsigned int total = 0;
  int result;
  while(total < length)
  {
    result = write(this->fileDescriptor, total+buffer, (size_t)length-total);
    if(result==0)
    {
      throw Comm::ConnectionFailure("This shouldn't happen");
    }
    if(result<0)
    {
      throw Comm::ConnectionFailure("Error writing data");
    }
    total+=result;
  }
  LOG_BUFFER("sending", buffer, length);
}

std::size_t Comm::Serial::Port::poll(unsigned char* buffer, std::size_t length){
  if(this->hasData())
  {
    int result = read(this->fileDescriptor, buffer, (size_t)length);
    LOG_BUFFER("reading", buffer, result);
    if(result>0)
      return result;
    throw Comm::ConnectionFailure("Disconnected from remote");
  }
  return 0;
}

void Comm::Serial::Port::configure(){
  // get config
  struct termios config;
  int successGettingConfig = tcgetattr(this->fileDescriptor, &config);
  if(successGettingConfig == -1){
    throw Comm::ConnectionFailure("Failed to connect to serial port. Failed to get config.");
  }

  //set baud rate
  int inputSpeedSupported = cfsetispeed(&config, this->speed);
  int outputSpeedSupported = cfsetospeed(&config, this->speed);
  if(inputSpeedSupported == -1 || outputSpeedSupported == -1){
    throw Comm::ConnectionFailure("Failed to connect to serial port. Failed to set baud rate.");
  }

  // set config bits (flags)
  // Parity = None | Stop bits = 1 | Clear bits size mask | Turn off hardware based flow control (RTS/CTS)
  config.c_cflag &= ~(PARENB|CSTOPB|CSIZE|CRTSCTS);
  // Set data bits size = 8 | Turn on the receiver | <- |
  config.c_cflag |= CS8|CREAD|CLOCAL;
  // Turn off software based flow control (XON/XOFF)
  config.c_iflag &= ~(IXON | IXOFF | IXANY);
  //config.c_iflag |= (IXON | IXOFF | IXANY);
  // Enable NON Cannonical mode
  config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  // Disable post-processing of bits
  config.c_oflag &= ~OPOST;
  // require at least one bit
  config.c_cc[VMIN] = 1;
  // instant timout for new bits (return when no new bits detected)
  config.c_cc[VTIME] = 0;

  cfmakeraw(&config);

  // apply config
  int successSettingConfig = tcsetattr(this->fileDescriptor, TCSANOW, &config);
  if(successSettingConfig == -1){
    throw Comm::ConnectionFailure("Failed to connect to serial port. Failed to apply config.");
  }

  int successFlushing = tcflush(this->fileDescriptor, TCIOFLUSH);
  if(successFlushing == -1){
    throw Comm::ConnectionFailure("Failed to connect to serial port. Failed to flush.");
  }
}

void Comm::Serial::Port::restartArduino(){
  // cause arduino to reset on connect
  // int RTS_flag;
  // RTS_flag = TIOCM_RTS;
  // ioctl(this->fileDescriptor,TIOCMBIS,&RTS_flag);//Set RTS pin
  // ioctl(this->fileDescriptor,TIOCMBIC,&RTS_flag);//Clear RTS pin
  int DTR_flag;
  DTR_flag = TIOCM_DTR;
  ioctl(this->fileDescriptor,TIOCMBIS,&DTR_flag);//Set RTS pin
  ioctl(this->fileDescriptor,TIOCMBIC,&DTR_flag);//Clear RTS pin
}
