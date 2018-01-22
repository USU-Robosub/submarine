#include <Tank.hpp>

TankLinear::TankLinear(Tank* tank)
  : tank(tank){}

void TankLinear::execute(){

}

void TankLinear::kill(){
  tank->kill();
}



TankTurn::TankTurn(Tank* tank)
  :tank(tank){}

void TankTurn::execute(){

}

void TankTurn::kill(){
  tank->kill();
}
