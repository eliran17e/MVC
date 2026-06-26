#include "Warehouse.h"

#include <iostream>

Warehouse::Warehouse(const std::string &n, std::shared_ptr<Point> location, int inventory)
    : Sim_obj(n), _inventory(inventory), _quantity(0), _location(std::make_shared<Point>(*location)) {
}
const shared_ptr<Point> &Warehouse::get_location() const {
    return _location;
}
void Warehouse::update() {
    _inventory+= _quantity; // Update inventory based on quantity
    _quantity = 0; // Reset quantity after updating inventory
}

void Warehouse::broadcast_current_state() {
    // Broadcast the current state of the warehouse
    std::cout << "Warehouse " << getName() << " at position (" << _location->x << ", " << _location->y << ") with inventory: " << _inventory << std::endl;
}

bool Warehouse::update_boxes(int quantity) {
    if (_inventory + quantity < 0) {
        std::cout << "Cannot remove more boxes than available in inventory." << std::endl;
        return false;
    } else {
        _quantity += quantity;
        return true;
    }
}
int Warehouse::getInventory() const {
    return _inventory;
}