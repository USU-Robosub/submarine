#ifndef SHOW_BOARD
#define SHOW_BOARD

#include <string>
#include <device/ShiftRegister.mock.hpp>

void initScreen();
void updateBoard();
void closeScreen();
void setSerialInfo(std::string port, int baud);
void displayShiftRegister(Mock::Device::ShiftRegister* shiftRegister, int shiftClockPin, int storageClockPin, int dataPin);

#endif
