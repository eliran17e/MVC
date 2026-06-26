#include "doctest.h"
#include "Geometry.h"

#include <cmath>

TEST_CASE("angle conversions are inverse of each other") {
    CHECK(to_radians(180.0) == doctest::Approx(M_PI));
    CHECK(to_degrees(M_PI) == doctest::Approx(180.0));
    CHECK(to_degrees(to_radians(57.3)) == doctest::Approx(57.3));
}

TEST_CASE("polar to cartesian conversion") {
    Polar_vector pv;
    pv.r = 5.0;
    pv.theta = 0.0;                 // pointing along +x
    Cartesian_vector cv(pv);
    CHECK(cv.delta_x == doctest::Approx(5.0));
    CHECK(cv.delta_y == doctest::Approx(0.0));
}

TEST_CASE("cartesian to polar normalizes angle to [0, 2pi)") {
    Cartesian_vector cv;
    cv.delta_x = 0.0;
    cv.delta_y = -1.0;              // straight down
    Polar_vector pv(cv);
    CHECK(pv.r == doctest::Approx(1.0));
    CHECK(pv.theta >= 0.0);
    CHECK(pv.theta == doctest::Approx(3.0 * M_PI / 2.0));
}

TEST_CASE("point equality") {
    Point a(1.0, 2.0);
    Point b(1.0, 2.0);
    Point c(1.0, 2.5);
    CHECK(a == b);
    CHECK_FALSE(a == c);
}
