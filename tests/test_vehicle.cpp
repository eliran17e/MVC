#include "doctest.h"
#include "Vehicle.h"

TEST_CASE("computeDistance returns euclidean distance") {
    CHECK(Vehicle::computeDistance(0.0, 0.0, 3.0, 4.0) == doctest::Approx(5.0));
    CHECK(Vehicle::computeDistance(1.0, 1.0, 1.0, 1.0) == doctest::Approx(0.0));
    CHECK(Vehicle::computeDistance(-1.0, -1.0, 2.0, 3.0) == doctest::Approx(5.0));
}
