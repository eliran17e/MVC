#include "doctest.h"
#include "Vehicle.h"
#include "State_trooper.h"
#include "Chopper.h"

#include <memory>

TEST_CASE("computeDistance returns euclidean distance") {
    CHECK(Vehicle::computeDistance(0.0, 0.0, 3.0, 4.0) == doctest::Approx(5.0));
    CHECK(Vehicle::computeDistance(1.0, 1.0, 1.0, 1.0) == doctest::Approx(0.0));
    CHECK(Vehicle::computeDistance(-1.0, -1.0, 2.0, 3.0) == doctest::Approx(5.0));
}

TEST_CASE("course 90 (east) advances +x using the compass convention") {
    State_trooper t("T", std::make_shared<Point>(0.0, 0.0));
    t.course(90.0, 100.0);   // 100 -> one world unit per tick
    t.update();
    CHECK(t.get_location()->x == doctest::Approx(1.0));
    CHECK(t.get_location()->y == doctest::Approx(0.0));
}

TEST_CASE("course 0 (north) advances +y") {
    State_trooper t("T", std::make_shared<Point>(0.0, 0.0));
    t.course(0.0, 100.0);
    t.update();
    CHECK(t.get_location()->x == doctest::Approx(0.0));
    CHECK(t.get_location()->y == doctest::Approx(1.0));
}

TEST_CASE("chopper in position mode heads toward its target") {
    Chopper c("C", std::make_shared<Point>(0.0, 0.0));
    c.position(0.0, 10.0, 100.0);   // due north, 10 units away
    c.update();
    CHECK(c.get_location()->x == doctest::Approx(0.0));
    CHECK(c.get_location()->y == doctest::Approx(1.0));   // moved one step closer
}
