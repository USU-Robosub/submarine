#include <Arduino.h>

int constrain(int x, int a, int b){
  return (x < a ? a : (x > b ? b : x));
}

void delay(int time){

}

Mock::Serial Serial;
