#ifndef TANK
#define TANK

#include <Controller.hpp>

class Tank {
  public:
    virtual void setLinear(int speed) = 0;
    virtual void setTurn(int speed) = 0;
    virtual void update() = 0;
    virtual void kill() = 0;
};

class TankLinear: public Controller {
  public:
    TankLinear(Tank* tank);
    virtual void execute();
    virtual void kill();

  private:
    Tank* tank;
};

class TankTurn: public Controller {
  public:
    TankTurn(Tank* tank);
    virtual void execute();
    virtual void kill();

  private:
    Tank* tank;
};

#endif
