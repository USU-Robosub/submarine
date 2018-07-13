#include <Arduino.h>
#include <Servo.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

extern void setup();
extern void loop();

void $printState();

int main(){
  std::cout << "Arduino Emulator v0.1" << std::endl;
  Mock::Arduino::$reset();
  std::cout << "Running setup()" << std::endl;
  setup();
  std::cout << "Starting loop()" << std::endl;
  while(true){
    loop();
    $printState();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}


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

void $printState(){
  for(int i = 0; i < ASCII_BOARD_SIZE; i++){
    if(i >= 3 && i < 16){
      int pin = i - 3;
      if(Servo::attached[pin]){
        int state = Mock::Arduino::digitalPinStates[i - 3];
        //std::cout << (state == NOT_SET ? "    " : (state == HIGH ? "HIGH" : " LOW")) << " ";
        std::cout << Servo::delay[pin] << " ";

        std::cout << " SERVO" << " ";
      }else{
        int state = Mock::Arduino::digitalPinStates[i - 3];
        std::cout << (state == NOT_SET ? "    " : (state == HIGH ? "HIGH" : " LOW")) << " ";

        int mode = Mock::Arduino::digitalPinModes[i - 3];
        std::cout << (mode == NOT_SET ? "      " : (mode == INPUT ? " INPUT" : "OUTPUT")) << " ";
      }
    }else{
      std::cout << "    " << " " << "      " << " ";
    }
    std::cout << asciiBoard[i];
    if(i >= 5 && i < 16){
      int pin = 23 - (i - 5);
      int mode = Mock::Arduino::digitalPinModes[23 - (i - 5)];
      std::cout << " " << (mode == NOT_SET ? "" : (mode == INPUT ? " INPUT" : "OUTPUT"));

      int state = Mock::Arduino::digitalPinStates[23 - (i - 5)];
      std::cout << " " << (state == NOT_SET ? "" : (state == HIGH ? "HIGH" : " LOW"));
    }
    std::cout << std::endl;
  }
}
