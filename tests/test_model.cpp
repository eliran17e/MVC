#include "doctest.h"
#include "Model.h"

TEST_CASE("parseTime converts HH:MM into fractional hours") {
    Model& model = Model::getInstance();
    CHECK(model.parseTime("8:00") == doctest::Approx(8.0));
    CHECK(model.parseTime("10:30") == doctest::Approx(10.5));
    CHECK(model.parseTime("0:15") == doctest::Approx(0.25));
}

TEST_CASE("parseTime returns 0 for malformed input") {
    Model& model = Model::getInstance();
    CHECK(model.parseTime("not-a-time") == doctest::Approx(0.0));
}

TEST_CASE("warehouses can be looked up by name") {
    Model& model = Model::getInstance();
    // The model seeds a default "Frankfurt" warehouse on construction.
    auto wh = model.findWarehouseByName("Frankfurt");
    REQUIRE(wh != nullptr);
    CHECK(wh->getName() == "Frankfurt");

    CHECK(model.findWarehouseByName("Nowhere") == nullptr);
}
