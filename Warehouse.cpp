//
// Created by User on 20/06/2025.
//

#include "Warehouse.h"
Warehouse::Warehouse(const string &n, std::shared_ptr<Point> location, int inventory)
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
    cout << "Warehouse " << getName() << " at location (" << _location->x << ", " << _location->y << ") with inventory: " << _inventory << endl;
}

void Warehouse::update_boxes(int quantity) {
    if (_inventory + quantity < 0) {
        cout << "Cannot remove more boxes than available in inventory." << endl;
    } else {
        _quantity += quantity;
    }
}