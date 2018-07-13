#ifndef SHOW_BOARD
#define SHOW_BOARD

#include <string>

void initScreen();
void updateBoard();
void closeScreen();
void setSerialInfo(std::string port, int baud);

#endif
