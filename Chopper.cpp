//
// Created by User on 20/06/2025.
//

#include "Chopper.h"
#include "Model.h"
#include "Truck.h"
#include "State_trooper.h"


void Chopper::update() {
    // Don't move if speed is 0
    if (speed == 0.0) return;

    // Compute movement vector based on angle and speed
    double radians = to_radians(angle);
    double dx = speed * std::sin(radians);
    double dy = speed * std::cos(radians);

    _location->x += dx;
    _location->y += dy;

    // If there is a destination, check if we’ve arrived
    if (destination) {
        double remaining = Vehicle::computeDistance(_location->x, _location->y,
                                                    destination->x, destination->y);

        if (remaining < 0.1) {
            _location = destination;
            destination = nullptr;
            speed = 0.0;
            setState("Stopped");
        }
    }
}


void Chopper::course(double angle, double speed) {
    this->angle = angle;
    this->speed = speed;
    this->destination = nullptr;
    setState("Patrolling on course " + std::to_string(angle));
}

void Chopper::position(double x, double y, double speed) {
    destination = std::make_shared<Point>(x, y);
    angle = to_degrees(std::atan2(x - _location->x, y - _location->y));
    this->speed = speed;
    setState("Moving to position");
}

bool Chopper::attack(const std::string& truckName) {
    const auto& vehicles = Model::getInstance().getVehicles();
    std::shared_ptr<Vehicle> targetTruck = nullptr;

    // Step 1: Find truck with the given name
    for (const auto& v : vehicles) {
        if (v->getName() == truckName && dynamic_cast<Truck*>(v.get()) != nullptr) {
            targetTruck = v;
            break;
        }
    }

    if (!targetTruck) {
        std::cerr << "Truck \"" << truckName << "\" not found.\n";
        setState("Stopped");
        return false;
    }

    // Step 2: Check distance to truck
    double truckDistance = Vehicle::computeDistance(_location->x,_location->y, targetTruck->get_location()->x, targetTruck->get_location()->y);
    if (truckDistance > attack_range) {
        std::cerr << "Truck is out of attack range.\n";
        attack_range = std::max(1, attack_range - 1);  // reduce attack range on failure
        setState("Stopped");
        return false;
    }

    // Step 3: Check for any police within 10km
    bool policeNearby = false;
    for (const auto& v : vehicles) {
        if (dynamic_cast<State_trooper*>(v.get()) != nullptr) {
            double dist = Vehicle::computeDistance(_location->x,_location->y, v->get_location()->x, v->get_location()->y);
            if (dist <= 10.0) {
                policeNearby = true;
                break;
            }
        }
    }

    // Final decision
    if (!policeNearby) {
        // Success: disable the truck
        auto* truck = dynamic_cast<Truck*>(targetTruck.get());
        truck->setState("Off road");
        truck->clearBoxes();       // zero boxes
        truck->setSpeed(0.0);      // stop the truck

        // Increase attack range by 1, max 20
        attack_range = std::min(attack_range + 1, 20);
        std::cout << "Attack succeeded. New attack range: " << attack_range << " km\n";

        setState("Stopped");
        return true;
    } else {
        // Failure due to nearby police
        attack_range = std::max(1, attack_range - 1);
        std::cerr << "Attack failed. Police nearby. Attack range reduced to: " << attack_range << " km\n";
        setState("Stopped");
        return false;
    }
}

