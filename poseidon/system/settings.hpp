#ifndef SYSTEM_SETTINGS
#define SYSTEM_SETTINGS

// arduino
#define ARDUINO_BAUD B115200
#define ARDUINO_PORT "1.2"
#define ARDUINO_PORT_EMULATED "/tmp/virtualcom1"

// agent
#define AGENT_PORT 3001
#define AGENT_DELIMITER '|'

// echo handler
#define ECHO_PORT_NUM 42

// dive Subsystem
#define DIVE_PORT_NUM 2
#define DIVE_PORT_NAME "dive"

// tank Subsystem
#define TANK_PORT_NUM 3
#define TANK_PORT_NAME "tank"

// global
#define LOOP_DELAY 1

#endif
