//
// Created by User on 20/06/2025.
//

#include "Chopper.h"
#include "Model.h"
#include "Truck.h"
#include "State_trooper.h"


void Chopper::update() {
    // Mode 0: Move to a position, then stop
    if (mode == 0) {
        if (!destination) {
            setState("Stopped");
            speed = 0.0;
            return;
        }
        double radians = to_radians(angle);
        double dy = (speed / 100.0) * std::cos(radians);
        double dx = (speed / 100.0) * std::sin(radians);
        _location->x += dx;
        _location->y += dy;

        double remaining = Vehicle::computeDistance(_location->x, _location->y, destination->x, destination->y);
        if (remaining < speed/100.0 ) {
            _location->x = destination->x;
            _location->y = destination->y;
            setState("Stopped");
            speed = 0.0;
            destination = nullptr;
        } else {
            setState("Moving to (" + std::to_string(destination->x) + "," + std::to_string(destination->y) + ")");
        }
        return;
    }

    // Mode 1: Move on a fixed course indefinitely
    if (mode == 1) {
        double radians = to_radians(angle);
        double dy = (speed / 100.0) * std::cos(radians);
        double dx = (speed / 100.0) * std::sin(radians);
        _location->x += dx;
        _location->y += dy;
        setState("Moving on course " + std::to_string(angle));
        return;
    }

    // Mode 2: Attack mode (try to attack a truck if target set)
    if (mode == 2) {
        // Attack logic is driven by Controller, so just respond to result
        if (attack_success) {
            // Success: disable the truck
            const auto truck = dynamic_cast<Truck*>(Model::getInstance().findVehicleByName(targetTruckName).get());
            truck->setState("Off road");
            truck->clearBoxes();       // zero boxes
            truck->setSpeed(0.0);      // stop the truck

            // Increase attack range by 1, max 20
            attack_range = std::min(attack_range + 0.01, 20.0);
            std::cout << "Attack succeeded. New attack range: " << attack_range << " km\n";
            setState("Stopped");
            this->speed = 0.0;
            mode = 0;

        } else {
            attack_range = std::max(0.01, attack_range - 0.01);  // reduce attack range on failure
            this->speed = 0.0;
            std::cerr << "Attack failed. Police nearby. Attack range reduced to: " << attack_range << " km\n";
            setState("Stopped");
        }

    }
}


void Chopper::course(double angle, double speed) {
    this->angle = angle;
    this->speed = speed;
    this->destination = nullptr;
    mode=1;
    setState("Moving to " + std::to_string(angle));
}

void Chopper::position(double x, double y, double speed) {
    destination = std::make_shared<Point>(x, y);
    angle = to_degrees(std::atan2(x - _location->x, y - _location->y));
    this->speed = speed;
    mode = 0; // position mode
    setState("Moving to position");
}

void Chopper::broadcast_current_state() {
    std::cout << "Chopper " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (destination && speed > 0.0)
        std::cout << "Heading on couse " << angle << "deg, ";

    std::cout << "Speed: " << speed << " km/h" << std::endl;
}

bool Chopper::attack(const std::string& truckName) {
    mode=2; // Set mode to attack
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
        attack_success = false;
        return false;
    }

    // Step 2: Check distance to truck
    double truckDistance = Vehicle::computeDistance(_location->x,_location->y, targetTruck->get_location()->x, targetTruck->get_location()->y);
    if (truckDistance > attack_range) {
        attack_success = false;
        return false;
    }

    // Step 3: Check for any police within 10km
    bool policeNearby = false;
    for (const auto& v : vehicles) {
        if (dynamic_cast<State_trooper*>(v.get()) != nullptr) {
            double dist = Vehicle::computeDistance(_location->x,_location->y, v->get_location()->x, v->get_location()->y);
            if (dist <= 0.1) {
                policeNearby = true;
                break;
            }
        }
    }

    // Final decision
    if (!policeNearby) {
        attack_success = true;
        targetTruckName = targetTruck->getName();
        return true;
    } else {
        // Failure due to nearby police
        attack_success = false;
        return false;
    }
}

