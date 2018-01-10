#include "catch.hpp"
#include "Math/Range.h"

TEST_CASE("can map one range to another", "[Range]"){
  SECTION("range set 1"){
    REQUIRE(
      Math::map(1,
        Math::Range{.min = 0, .max = 5},
        Math::Range{.min = -250, .max = 250}
      ) == -150
    );
  }

  SECTION("range set 2"){
    REQUIRE(
      Math::map(-0.5,
        Math::Range{.min = -1, .max = 1},
        Math::Range{.min = 0, .max = 400}
      ) == 100
    );
  }
}

TEST_CASE("can bound a value inside a range", "[Range]"){
  SECTION("range set 1"){
    REQUIRE( Math::bound(10,Math::Range{.min = 0, .max = 5}) == 5 );
  }

  SECTION("range set 2"){
    REQUIRE( Math::bound(-6.254,Math::Range{.min = 10, .max = 12}) == 10 );
  }
}
