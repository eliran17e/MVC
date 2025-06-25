//
// Created by User on 20/06/2025.
//

#include "Chopper.h"
#include "Model.h"
#include "Truck.h"
#include "State_trooper.h"


void Chopper::update() {

    // Handle pending manual commands
    if (pending_position) {
        // Setup for position mode
        this->destination = std::make_shared<Point>(pending_pos_x, pending_pos_y);
        double dx = pending_pos_x - _location->x;
        double dy = pending_pos_y - _location->y;
        this->angle = to_degrees(std::atan2(dx, dy));
        this->speed = pending_pos_speed;
        this->mode = 0;
        this->en_route = true;
        setState("Moving to position");
        pending_position = false;
    }

    if (pending_course) {
        this->angle = pending_course_angle;
        this->speed = pending_course_speed;
        this->destination = nullptr;
        this->mode = 1;
        this->en_route = true;
        setState("Moving to " + std::to_string(angle));
        pending_course = false;
    }

    if (state == "Stopped") {
        this->stop();
        return;
    }
    // Mode 0: Move to a position, then stop
    if (mode == 0) {
        if (!destination) {
            setState("Stopped");
            en_route = false;
            speed = 0.0;
        }
        else {
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
                en_route = false;
                speed = 0.0;
                destination = nullptr;
            } else {
                setState("Moving to (" + std::to_string(destination->x) + "," + std::to_string(destination->y) + ")");
            }
        }
    }

    // Mode 1: Move on a fixed course indefinitely
    if (mode == 1 && en_route) {
        double radians = to_radians(angle);
        double dy = (speed / 100.0) * std::cos(radians);
        double dx = (speed / 100.0) * std::sin(radians);
        _location->x += dx;
        _location->y += dy;
        setState("Moving on course " + std::to_string(angle));
    }

    // Check if an attack is pending
    if (pending_attack) {
        // Find the target truck and police
        const auto& vehicles = Model::getInstance().getVehicles();
        std::shared_ptr<Vehicle> targetTruck = nullptr;

        for (const auto& v : vehicles) {
            if (v->getName() == pending_attack_target && dynamic_cast<Truck*>(v.get()) != nullptr) {
                targetTruck = v;
                break;
            }
        }

        // Default result: fail
        attack_success = false;

        if (targetTruck) {
            double truckDistance = Vehicle::computeDistance(_location->x, _location->y,
                                                            targetTruck->get_location()->x, targetTruck->get_location()->y);

            if (truckDistance < attack_range) {
                // Check for police
                bool policeNearby = false;
                for (const auto& v : vehicles) {
                    if (dynamic_cast<State_trooper*>(v.get()) != nullptr) {
                        double dist = Vehicle::computeDistance(targetTruck->get_location()->x,
                                                               targetTruck->get_location()->y,
                                                               v->get_location()->x,
                                                               v->get_location()->y);
                        if (dist <= 0.1) {
                            policeNearby = true;
                            break;
                        }
                    }
                }

                if (!policeNearby) {
                    attack_success = true;
                    targetTruckName = targetTruck->getName();
                }
            }
        }

        // Reset the pending flag
        pending_attack = false;

        mode = 2;
    }

    // Mode 2: Attack mode (try to attack a truck if target set)
    if (mode == 2) {
        // Attack logic is driven by Controller, so just respond to result
        if (attack_success) {
            // Success: disable the truck
            const auto truck = dynamic_cast<Truck*>(Model::getInstance().findVehicleByName(targetTruckName).get());
            truck->stop();
            truck->setState("Off road");
            truck->clearBoxes();       // zero boxes
            truck->setSpeed(0.0);      // stop the truck


            // Increase attack range by 1, max 20
            attack_range = std::min(attack_range + 0.01, 20.0);
            std::cout << "Attack succeeded. New attack range: " << attack_range*100 << " km\n";
            setState("Stopped");
            this->en_route = false;
            this->speed = 0.0;
            mode = 0;

        } else {
            attack_range = std::max(0.01, attack_range - 0.01);  // reduce attack range on failure
            this->speed = 0.0;
            this->en_route = false;
            std::cout << "Attack failed. Attack range reduced to: " << attack_range*100 << " km\n";
            setState("Stopped");
        }

    }
}


void Chopper::broadcast_current_state() {
    std::cout << "Chopper " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (mode == 1 && en_route) {
        std::cout << "Moving on course " << std::fmod(angle, 360.0) << " deg, speed: " << speed  << " km/h. "<<std::endl;
    }
    else if (mode == 0 && en_route) {
        std::cout << "Moving to position (" << destination->x << ", " << destination->y << "), speed: " << speed << " km/h. "<<std::endl;
    }
    else {
        std::cout << this->state << "." << std::endl;
    }

}

bool Chopper::attack(const std::string& truckName) {
    // Schedule attack for the next update step
    pending_attack = true;
    pending_attack_target = truckName;
    mode = 2; // attack mode
    setState("Attacking " + truckName);
    return true; // you can ignore this, the actual result is checked in update
}

