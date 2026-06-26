#include "doctest.h"
#include "Warehouse.h"

#include <memory>

TEST_CASE("warehouse buffers inventory changes until update()") {
    Warehouse wh("Depot", std::make_shared<Point>(0.0, 0.0), 100);
    REQUIRE(wh.getInventory() == 100);

    // Scheduled changes are not visible until update() applies them.
    CHECK(wh.update_boxes(-30) == true);
    CHECK(wh.getInventory() == 100);

    wh.update();
    CHECK(wh.getInventory() == 70);
}

TEST_CASE("warehouse rejects removing more than is in stock") {
    Warehouse wh("Depot", std::make_shared<Point>(0.0, 0.0), 50);

    CHECK(wh.update_boxes(-200) == false);
    wh.update();
    CHECK(wh.getInventory() == 50);   // unchanged
}

TEST_CASE("warehouse accepts incoming stock") {
    Warehouse wh("Depot", std::make_shared<Point>(0.0, 0.0), 0);

    CHECK(wh.update_boxes(40) == true);
    wh.update();
    CHECK(wh.getInventory() == 40);
}
