#include <Tank.hpp>

TankLinear::TankLinear(*Tank tank)
  : tank(tank){}

virtual void TankLinear::execute(){

}

virtual void TankLinear::kill(){
  tank->kill();
}



TankTurn::TankTurn(*Tank tank)
  :tank(tank){}

virtual void TankTurn::execute(){

}

virtual void TankTurn::kill(){
  tank->kill();
}
