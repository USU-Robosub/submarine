// #include "catch.hpp"
// #include "Device/Thruster.h"
//
// TEST_CASE("thruster creates motor command correctly", "[Thruster]"){
//   observable<int> power = observable<>::create<int>([](subscriber<int> s){
//     s.on_next(123);
//     s.on_next(254);
//     s.on_completed();
//   });
//
//   observable<std::vector<int>> commands = Device::Thruster::getSerialCommands(power, 12);
//
//   commands.element_at(0).subscribe([](std::vector<int> command){
//     REQUIRE( command.size() == 2 );
//     REQUIRE( command[0] == 12 );
//     REQUIRE( command[1] == 123 );
//   });
//   commands.element_at(1).subscribe([](std::vector<int> command){
//     REQUIRE( command.size() == 2 );
//     REQUIRE( command[0] == 12 );
//     REQUIRE( command[1] == 254 );
//   });
// }
