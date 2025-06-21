//
// Created by User on 20/06/2025.
//

#include "State_trooper.h"


void State_trooper::course(double angle,double speed) {
    this->angle = angle;
    this->speed = speed;
    this->destination = nullptr;
    en_route = true;
    arrived = false;
    setState("Patrolling on course " + std::to_string(angle));
}

void State_trooper::position(double x, double y,double speed) {
    destination = std::make_shared<Point>(x, y);
    angle = to_degrees(std::atan2(x - _location->x, y - _location->y));
    this->speed = speed;
    en_route = true;
    arrived = false;
    setState("Moving to position");
}


void State_trooper::goToDestination(const std::string& warehouseName) {
    const auto& warehouses = Model::getInstance().getWarehouses();

    for (const auto& wh : warehouses) {
        if (wh->getName() == warehouseName) {
            destination = wh->get_location();
            angle = to_degrees(std::atan2(destination->x - _location->x,
                                          destination->y - _location->y));
            speed = 90.0;
            en_route = true;
            arrived = false;
            currentDestinationName = warehouseName;
            visitedWarehouses.insert(warehouseName);
            setState("Heading to " + warehouseName);
            return;
        }
    }

    std::cerr << "Warehouse \"" << warehouseName << "\" not found.\n";
}


void State_trooper::update() {
    if (!en_route || !destination) return;

    double radians = to_radians(angle);
    double dx = speed * std::sin(radians);
    double dy = speed * std::cos(radians);

    _location->x += dx;
    _location->y += dy;

    double remaining = Vehicle::computeDistance(_location->x, _location->y,
                                                destination->x, destination->y);

    if (remaining < 0.1 && !arrived) {
        _location = destination;
        setState("Arrived at " + currentDestinationName);
        en_route = false;
        arrived = true;

        startPatrol();

    }
}

void State_trooper::broadcast_current_state() {
    std::cout << "State_trooper " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (destination && !arrived)
        std::cout << "Heading to " << currentDestinationName << ", ";

    std::cout << "Speed: " << speed << " km/h" << std::endl;
}


void State_trooper::startPatrol() {
    const auto& warehouses = Model::getInstance().getWarehouses();
    std::string nextName;
    std::shared_ptr<Point> nextLocation = nullptr;
    double minDist = 1e9;

    for (const auto& wh : warehouses) {
        if (visitedWarehouses.count(wh->getName()) == 0) {
            double dist = Vehicle::computeDistance(_location->x, _location->y,
                                                   wh->get_location()->x, wh->get_location()->y);

            if ((dist < minDist) || (dist == minDist && wh->getName() < nextName)) {
                minDist = dist;
                nextName = wh->getName();
                nextLocation = wh->get_location();
            }
        }
    }

    if (nextLocation) {
        destination = nextLocation;
        angle = to_degrees(std::atan2(destination->x - _location->x,
                                      destination->y - _location->y));
        speed = 90.0;
        en_route = true;
        arrived = false;
        currentDestinationName = nextName;
        visitedWarehouses.insert(nextName);
        setState("Patrolling to " + nextName);
    } else {
        // All warehouses visited
        destination = nullptr;
        speed = 0.0;
        setState("Stopped");
    }
}
