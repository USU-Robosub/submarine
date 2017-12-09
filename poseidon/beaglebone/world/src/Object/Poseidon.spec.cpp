#include "catch2/catch.hpp"
#include "Object/Poseidon.h"
#include "Device/Thruster.h"
#include "Comm/Hub.mock.h"

TEST_CASE("abc", "[Poseidon]"){
  Comm::TestHub hub;
  Object::Poseidon::Config config{
    .frontThruster = Device::Thruster(&hub, 0)
  };
  Object::Poseidon poseidon();
}

TEST_CASE("cde", "[Poseidon]"){
  World["Poseidon"]["frontThruster"]
}

/*

{
  World: {
    Poseidon: {
      frontThruster: {
        power: 0.3
      }
    }
  }
}

{
  type: "MotorSetPower",
  name: ["poseidon", "frontThruster"],
  power: 0.3
}

world["poseidon"].pose.x();
world["poseidon"].frontThruster.power(0.3);
world["poseidon"].motion.forward(0.3);
world["poseidon"].attitudeControl.yaw(2.3);
world["poseidon"].accelerometer.x().subscribe();
world["poseidon"].accelerometer.x().subscribe();
world["poseidon"].accelerometer.x();
world.get<Poseidon>("pogen2")->compass->heading().subscribe([](double angle){
  world.get<Poseidon>("pogen2")->motion->turn(
    headingPID.correctFor(angle, stopwatch.messure())
  );
});

accelerometer -> mouse position
thruster -> element color

state

action -> create new state -> update
subject.on_next() -> reducer -> observable.subscribe()

poseidon.motion.forward = power => {
  hub.dispatch({
    type: "motionForward",
    objectName: "poseidon"
    power: power
  })
}

function motion = objectName => (motion, action) => {
  if(action.type == "motionForward" && action.objectName == " objectName"){
    motion.forward(action.power);
  }
}

void motionReducer(Motion motion, Json action){
  if(){

  }
}

hub.add();

hub.on("object name").on("sub system").on("operation").subscribe(action => {

})

Poseidon.attachTo(hub) => {
  poseidonHub = hub.on(name);
  poseidonHub.subscribe(action => {
    if(action.type == ".."){ ... }
    ...
  });

  children.forEach(child => {
    child.attachTo(poseidonHub);
  });
}

class Reducer{
public:
  virtual std::string getName() = 0;
  virtual std::vector<Reducer> getChildren() = 0;
  vitrual void reduce(Json action) = 0;
};

class ThrusterReducer : public Reducer{
public:
  Poseidon(std::string name, Thruster* thruster) : thruster(thruster), name(name) { }
  std::string getName() { return this-> name; }
  std::vector<Reducer> getChildren() { return this->children; }
  void reduce(Json action) {
    if(action["type"] == "SET_POWER"){
      this->thruster->setPower(action["power"]);
    }
  }
private:
  Thruster* thruster;
  std::string name;
  std::vector<Reducer> children;
}

class AccelerometerReducer : public Reducer{
public:
  Poseidon(std::string name, Accelerometer* accelerometer) : accelerometer(accelerometer), name(name) { }
  std::string getName() { return this-> name; }
  std::vector<Reducer> getChildren() { return this->children; }
  void reduce(Json action) {
    if(action["type"] == "GET_ACCELERATION"){
      this->accelerometer->getAcceleration(action["power"]);
    }
  }
  void attachTo(RemoteStore remoteStore){
    remoteStore.actions.subscribe([=](Json action){
      if(action["type"] == "GET_ACCELERATION"){
        this->accelerometer->getAcceleration(action["power"]);
      }
    });
  }
private:
  Accelerometer* accelerometer;
  std::string name;
  std::vector<Reducer> children;
}







const poseidon = {
  thrusters: {
    front: ,
    back: ,
    left: ,
    right:
  },
  accelerometer: ,
  compass ,
  gyroscope ,
  temperature: {
    internal,
    external
  },
  pressure: {
    internal,
    external
  },
  voltage ,
  switchs: {
    action: ,
    kill: ,
  }
}

*/

struct NineAxisIMU{
  Accelerometer* accelerometer();
  Compass* compass();
  Gyroscope* gyroscope();
};

namespace Poseidon{

  struct Arm{
    Motor* first;
    Motor* second;
    Motor* third;
  };

  struct PropulsionThrusters{
    Motor* left;
    Motor* right;
  };

  struct DiveThrusters{
    Motor* front;
    Motor* back;
  };

  struct Switches{
    Switch* action;
    Switch* kill;
  };

  struct TemperatureAndPressure{
    TemperatureSensor* temperatureSensor;
    PressureSensor* PressureSensor;
  };

  struct Submarine{
    PropulsionThrusters propulsionThrusters();
    DiveThrusters diveThrusters();
    NineAxisIMU imu();
    TemperatureAndPressure internal();
    TemperatureAndPressure external();
    VoltageSensor voltage();
    Switches switches();
    Arm arm();
  };

  // class Submarine{
  // public:
  //   Submarine(Hardware config) : config(config) { }
  //   Hardware hardware() { return this->config; }
  // private:
  //   Hardware config;
  // };

}

Comm::Hub hub;
Timing::Stopwatch stopwatch;

Poseidon::PropulsionThrusters propulsionThrusters {
  .left = new Device::Thruster(&hub, 10),
  .right = new Device::Thruster(&hub, 11)
};

Poseidon::Submarine poseidon {
  .propulsionThrusters = propulsionThrusters,
  .diveThrusters = ,
  .imu = ,
  .internal = ,
  .external = ,
  .voltage = ,
  .switches = ,
  .arm =
};

PID::GainConfig gains { .p = 1, .i = 2, .d = 3 };

PID::Controller yawPID(gains, 4);

poseidon.propulsionThrusters.left->power(0.3);

while(/* ... */){
  double heading = poseidon.imu.compass->value().heading;
  double turnSpeed = yawPID.correctFor(heading, stopwatch.messure());
  poseidon.propulsionThrusters.left->power(turnSpeed);
  poseidon.propulsionThrusters.right->power(-turnSpeed);
}

Control::MergedMotor dive(
  poseidon.diveThrusters.front,
  poseidon.diveThrusters.back
);

dive.setPower(0.5);

Control::DualDrive dualDrive(
  poseidon.propulsionThrusters.left,
  poseidon.propulsionThrusters.right
);

dualDrive.forward(0.3);

while(/* ... */){
  double heading = poseidon.imu.compass->value().heading;
  double turnSpeed = yawPID.correctFor(heading, stopwatch.messure());
  dualDrive.turn(turnSpeed);
}

delete propulsionThrusters.left;
delete propulsionThrusters.right;
