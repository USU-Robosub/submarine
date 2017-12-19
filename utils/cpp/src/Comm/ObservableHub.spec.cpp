#include "catch.hpp"
#include "Comm/Bridge.mock.hpp"
#include "Comm/ObservableHub.h"
#include <sstream>

struct TestStruct{
  double value1;
  int value2;
};

TEST_CASE("ObservableHub", "[ObservableHub]"){
  // Comm::Mock::TestBridge bridge;
  // Comm::ObservableHub hub(&bridge);
  // observable<TestStruct> testObservable = observable<>::create<TestStruct>([](subscriber<TestStruct> s){
  //   s.on_next(TestStruct{.value1=10.4,.value2=5});
  //   s.on_completed();
  // });
  // hub.publish("test", testObservable.map([](TestStruct data) -> std::string{
  //   std::stringstream ss;
  //   ss << data.value1 << " " << data.value2;
  //   return ss.str();
  // }));
  //REQUIRE( bridge.writeQueue.front() ==  );
}
