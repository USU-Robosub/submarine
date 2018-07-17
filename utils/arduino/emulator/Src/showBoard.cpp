#include <showBoard.hpp>

#include <curses.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <Servo.h>
#include <Arduino.h>
#include <sstream>
#include <iomanip>


void initScreen();
void drawCube();
void closeScreen();
void signalHandler(int signal);
void drawLine(int x1, int y1, int x2, int y2);

int width = 0, height = 0;
bool changed = true;
#define ASCII_BOARD_SIZE 17
std::string asciiBoard[ASCII_BOARD_SIZE] = {
  "             +-----+             ",
  "+------------| USB |------------+",
  "| []GND      +-----+      Vin[] |",
  "| []RX1/0                AGND[] |",
  "| []TX1/1                 3V3[] |",
  "| []2                   23/A9[] |",
  "| []3                   22/A8[] |",
  "| []4                   21/A7[] |",
  "| []5        |||||||    20/A6[] |",
  "| []6       -+-----+-   19/A5[] |",
  "| []RX3/7   -| CPU |-   18/A4[] |",
  "| []TX3/8   -+-----+-   17/A3[] |",
  "| []RX2/9    |||||||    16/A2[] |",
  "| []TX2/10              15/A1[] |",
  "| []11                  14/A0[] |",
  "| []12   TEENSY 3.1    13/LED[] |",
  "+-------------------------------+",
};

#define SHIFT_REGISTER_CHIP_SIZE 10
std::string shiftRegisterChip[SHIFT_REGISTER_CHIP_SIZE] = {
  "+-----\\__/-----+",
  "| []1    VCC[] |",
  "| []2      0[] |",
  "| []3     DS[] |",
  "| []4     OE[] |",
  "| []5   STCP[] |",
  "| []6   SHCP[] |",
  "| []7     MR[] |",
  "| []GND   7'[] |",
  "+--------------+",
};

void initScreen(){
  signal(SIGWINCH, signalHandler);

  initscr(); // start curses
  width = COLS;
  height = LINES;
  (void)noecho();
  //nonl();
  cbreak();
  //halfdelay(1);
  nodelay(stdscr, TRUE);
  curs_set(0);
}

void closeScreen(){
  endwin(); // end curses
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

std::string serialPath = "Not Connected";
int serialBaud = 0;
void setSerialInfo(std::string port, int baud){
  serialPath = port;
  serialBaud = baud;
}

Mock::Device::ShiftRegister* _shiftRegister = nullptr;
int _shiftClockPin;
int _storageClockPin;
int _dataPin;

void displayShiftRegister(Mock::Device::ShiftRegister* shiftRegister, int shiftClockPin, int storageClockPin, int dataPin){
  _shiftRegister = shiftRegister;
  _shiftClockPin = shiftClockPin;
  _storageClockPin = storageClockPin;
  _dataPin = dataPin;
}

void updateBoard(){
  if(changed){
    clear();
    changed = false;
  }
  //clear();
  int infoX = 1;// width/2 - 46/2;
  // print title
  mvprintw(1, infoX, "==============================================");
  mvprintw(2, infoX, "==          Arduino Emulator v0.1           ==");
  mvprintw(3, infoX, "==============================================");
  mvprintw(4, infoX, "Press [q] to exit");
  // print serial info
  mvprintw(6, infoX, "-- Serial Port Info --");
  mvprintw(7, infoX, ("Connected to port: " + serialPath).c_str());
  mvprintw(8, infoX, ("Running at baud: " + std::to_string(serialBaud)).c_str());
  int boardY = height/2 - ASCII_BOARD_SIZE/2;
  if(boardY < 10){
    boardY = 10;
  }
  int boardX = width/2 - asciiBoard[0].length()/2;
  // print board
  for(int i = 0; i < ASCII_BOARD_SIZE; i++){
    mvprintw(boardY + i, boardX, asciiBoard[i].c_str());
  }
  // print servos
  for(int i = 0; i < 24; i++){
    if(i <= 12){
      mvprintw(boardY + 3 + i, boardX - 20, "                    ");
    }else{
      mvprintw(boardY + 3 + i, boardX + asciiBoard[0].length(), "                    ");
    }
    if(Servo::attached[i]){
      if(i <= 12){
        mvprintw(boardY + 3 + i, boardX - 6, "Servo");
        mvprintw(boardY + 3 + i, boardX - 6 - 5, std::to_string(Servo::delay[i]).c_str());
        mvprintw(boardY + 3 + i, boardX - 6 - 5 - 6, to_string_with_precision((float)(Servo::delay[i] - 1500) / 400.0f, 2).c_str());
      }else{
        mvprintw(boardY + 5 + 10 - (i - 13), boardX + asciiBoard[0].length() + 1, "Servo");
        mvprintw(boardY + 5 + 10 - (i - 13), boardX + asciiBoard[0].length() + 1 + 6, std::to_string(Servo::delay[i]).c_str());
      }
    }else{
      if(i <= 12){
        if(Mock::Arduino::digitalPinModes[i] == OUTPUT){
          mvprintw(boardY + 3 + i, boardX - 7, "OUTPUT");
          mvprintw(boardY + 3 + i, boardX - 7 - 5, (Mock::Arduino::digitalPinStates[i] == HIGH ? "HIGH" : "LOW"));
        }
      }else{
        // TODO
      }
    }
  }

  int shiftY = boardY + ASCII_BOARD_SIZE + 1;
  int shiftX = width/2 - shiftRegisterChip[0].length()/2;
  // print board
  for(int i = 0; i < SHIFT_REGISTER_CHIP_SIZE; i++){
    mvprintw(shiftY + i, shiftX, shiftRegisterChip[i].c_str());
  }
  // mvprintw(shiftY + 1, shiftX + shiftRegisterChip[0].length() + 1, "+5V");
  // mvprintw(shiftY + SHIFT_REGISTER_CHIP_SIZE - 2, shiftX - 4, "+0V");
  mvprintw(shiftY + SHIFT_REGISTER_CHIP_SIZE - 4, shiftX + shiftRegisterChip[0].length() + 1, ("PIN #" + std::to_string(_shiftClockPin)).c_str());
  mvprintw(shiftY + SHIFT_REGISTER_CHIP_SIZE - 5, shiftX + shiftRegisterChip[0].length() + 1, ("PIN #" + std::to_string(_storageClockPin)).c_str());
  mvprintw(shiftY + 3, shiftX + shiftRegisterChip[0].length() + 1, ("PIN #" + std::to_string(_dataPin)).c_str());
  // print shiftRegister
  for(int i = 0; i < 8; i++){
    if(i == 0){
      mvprintw(shiftY + 2 + i, shiftX + shiftRegisterChip[0].length(), "                    ");
      mvprintw(shiftY + 2 + i, shiftX + shiftRegisterChip[0].length() + 1, (_shiftRegister->pins[i] == HIGH ? "HIGH" : "LOW"));
    }else{
      mvprintw(shiftY + 1 + i - 1, shiftX - 20, "                    ");
      mvprintw(shiftY + 1 + i - 1, shiftX - 5, (_shiftRegister->pins[i] == HIGH ? "HIGH" : "LOW"));
    }
  }
  refresh();
  std::this_thread::sleep_for (std::chrono::milliseconds(50));
}

void signalHandler(int signal)
{
  if(signal == SIGWINCH){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
    if(is_term_resized(height, width)){
      changed = true;
      resizeterm(height, width);
    }
  }
}
