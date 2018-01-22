#ifndef CONTROLLER
#define CONTROLLER

class Controller {
  public:
    virtual void execute() = 0;
    virtual void kill() = 0;
};

#endif
