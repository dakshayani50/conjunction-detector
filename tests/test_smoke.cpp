#include <catch2/catch_test_macros.hpp>
#include "core.hpp"

TEST_CASE("library reports a version") {
    REQUIRE(cdet::version() == "0.1.0");
}
