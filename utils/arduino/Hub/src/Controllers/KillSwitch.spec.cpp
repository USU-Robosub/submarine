#include <catch2/catch.hpp>
#include <Controllers/KillSwitch.hpp>
#include <Emitter.mock.hpp>

TEST_CASE("happy path", "[killSwitch]"){
  Controllers::KillSwitch killSwitch(3, 13, nullptr);
  int32_t buffer[3] = {0};
  Mock::Emitter emitter(buffer);
  killSwitch.use(&emitter);
}
