#include <Arduino.h>
#include <Servo.h>
#include <thread>
#include <chrono>
//#include <iostream>
#include <iomanip>
#include <SerialPort.hpp>
#include <showBoard.hpp>

extern void setup();
extern void loop();

int get_baud(int baud);

void $printState();

SerialPort* serialPort;
std::string serialPortPath;

void begin(int baud) {
  //std::cout << "Start serial with baud=" << baud << std::endl;
  serialPort = new SerialPort(serialPortPath, get_baud(baud));
  setSerialInfo(serialPortPath, baud);
}

int available(){
  //std::cout << "Serial check available" << std::endl;
  bool hasData = serialPort->hasData();
  //std::cout << "Serial " << (hasData ? "has data" : "is empty") << std::endl;
  return (hasData ? 4 : 0);
}

void write(unsigned char* buffer, long length){
  //std::cout << "Serial write [" << length << "] ";
  for(int i = 0; i < length; i++){
    //std::cout << buffer[i] << " ";
  }
  //std::cout << std::endl;
  serialPort->push(buffer, length);
}

int16_t read() {
  //std::cout << "Serial read" << std::endl;
  unsigned char buffer[1] = {0};
  serialPort->poll(buffer, 1);
  return buffer[0];
}

#include<iostream>

#include <curses.h>
bool quit = false;
void readConsole(){
  while(!quit){
    char c = getch();
    if(c == ERR){
      continue;
    }
    std::string s;
    s.push_back(c);
    mvprintw(20, 1, s.c_str());
    if(c == 'q'){
      quit = true;
    }
  }
}

int main(int argc, char** args){
  if(argc != 2){
    //std::cout << "Use instead: command [path to serial port]";
    return 1;
  }
  initScreen();
  //std::thread charIn(readConsole);

  serialPortPath = args[1];
  Serial.$intercept(read, write, available, begin);

  //std::cout << "Arduino Emulator v0.1" << std::endl;
  Mock::Arduino::$reset();
  Mock::Arduino::$enableMillis();
  Mock::Device::ShiftRegister shiftRegister(10, 11, 12);
  displayShiftRegister(&shiftRegister, 10, 11, 12);
  //std::cout << "Running setup()" << std::endl;
  setup();
  //std::cout << "Starting loop()" << std::endl;
  //bool quit = false;
  while(!quit){
    loop();
    char c = getch();
    if(c != ERR){
      std::string s("Input Code: ");
      s.push_back(c);
      mvprintw(LINES - 2, 1, s.c_str());
      if(c == 'q'){
        quit = true;
      }else if(c == 'p'){
        mvprintw(10, COLS/2 - 8, "--Pin Set Mode--");
        mvprintw(11, COLS/2 - 8, "Select pin to set");
      }
    }
    //$printState();
    updateBoard();
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  closeScreen();
  //charIn.join();
  return 0;
}


// #define ASCII_BOARD_SIZE 17
// std::string asciiBoard[ASCII_BOARD_SIZE] = {
//   "             +-----+             ",
//   "+------------| USB |------------+",
//   "| []GND      +-----+      Vin[] |",
//   "| []RX1/0                AGND[] |",
//   "| []TX1/1                 3V3[] |",
//   "| []2                   23/A9[] |",
//   "| []3                   22/A8[] |",
//   "| []4                   21/A7[] |",
//   "| []5        |||||||    20/A6[] |",
//   "| []6       -+-----+-   19/A5[] |",
//   "| []RX3/7   -| CPU |-   18/A4[] |",
//   "| []TX3/8   -+-----+-   17/A3[] |",
//   "| []RX2/9    |||||||    16/A2[] |",
//   "| []TX2/10              15/A1[] |",
//   "| []11                  14/A0[] |",
//   "| []12   TEENSY 3.1    13/LED[] |",
//   "+-------------------------------+",
// };

// void $printState(){
//   for(int i = 0; i < ASCII_BOARD_SIZE; i++){
//     if(i >= 3 && i < 16){
//       int pin = i - 3;
//       if(Servo::attached[pin]){
//         int state = Mock::Arduino::digitalPinStates[i - 3];
//         //std::cout << (state == NOT_SET ? "    " : (state == HIGH ? "HIGH" : " LOW")) << " ";
//         std::cout << Servo::delay[pin] << " ";
//
//         std::cout << " SERVO" << " ";
//       }else{
//         int state = Mock::Arduino::digitalPinStates[i - 3];
//         std::cout << (state == NOT_SET ? "    " : (state == HIGH ? "HIGH" : " LOW")) << " ";
//
//         int mode = Mock::Arduino::digitalPinModes[i - 3];
//         std::cout << (mode == NOT_SET ? "      " : (mode == INPUT ? " INPUT" : "OUTPUT")) << " ";
//       }
//     }else{
//       std::cout << "    " << " " << "      " << " ";
//     }
//     std::cout << asciiBoard[i];
//     if(i >= 5 && i < 16){
//       int pin = 23 - (i - 5);
//       int mode = Mock::Arduino::digitalPinModes[23 - (i - 5)];
//       std::cout << " " << (mode == NOT_SET ? "" : (mode == INPUT ? " INPUT" : "OUTPUT"));
//
//       int state = Mock::Arduino::digitalPinStates[23 - (i - 5)];
//       std::cout << " " << (state == NOT_SET ? "" : (state == HIGH ? "HIGH" : " LOW"));
//     }
//     std::cout << std::endl;
//   }
// }

int get_baud(int baud)
{
    switch (baud) {
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    case 500000:
        return B500000;
    case 576000:
        return B576000;
    case 921600:
        return B921600;
    case 1000000:
        return B1000000;
    case 1152000:
        return B1152000;
    case 1500000:
        return B1500000;
    case 2000000:
        return B2000000;
    case 2500000:
        return B2500000;
    case 3000000:
        return B3000000;
    case 3500000:
        return B3500000;
    case 4000000:
        return B4000000;
    default:
        return -1;
    }
}
