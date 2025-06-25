//
// Created by User on 20/06/2025.
//

#include "State_trooper.h"


// void State_trooper::course(double angle,double speed = 90.0) {
//     this->angle = angle;
//     this->speed = speed;
//     this->destination = nullptr;
//     en_route = true;
//     arrived = false;
//     mode = 1; // <-- course mode
//     setState("Moving to " + std::to_string(angle));
// }
//
// void State_trooper::position(double x, double y,double speed=90.0) {
//     destination = std::make_shared<Point>(x, y);
//     angle = to_degrees(std::atan2(x - _location->x, y - _location->y));
//     this->speed = speed;
//     en_route = true;
//     arrived = false;
//     mode = 0; // <-- position mode
//     setState("Moving to position");
// }


void State_trooper::goToDestination(const std::string& warehouseName) {
    pending_goto = true;
    pending_goto_name = warehouseName;
}


void State_trooper::update() {


    if (pending_position) {
        destination = std::make_shared<Point>(pending_pos_x, pending_pos_y);
        double dx = pending_pos_x - _location->x;
        double dy = pending_pos_y - _location->y;
        this->angle = to_degrees(std::atan2(dx, dy));
        this->speed = pending_pos_speed;
        this->mode = 0;
        this->en_route = true;
        this->arrived = false;
        setState("Moving to position");
        pending_position = false;
    }
    if (pending_course) {
        this->angle = pending_course_angle;
        this->speed = pending_course_speed;
        this->destination = nullptr;
        this->mode = 1;
        this->en_route = true;
        this->arrived = false;
        setState("Moving to " + std::to_string(angle));
        pending_course = false;
    }
    if (pending_goto) {
        const auto& warehouses = Model::getInstance().getWarehouses();
        for (const auto& wh : warehouses) {
            if (wh->getName() == pending_goto_name) {
                destination = wh->get_location();
                angle = to_degrees(std::atan2(destination->x - _location->x,
                                              destination->y - _location->y));
                speed = 90.0;
                en_route = true;
                arrived = false;
                mode = 2;
                currentDestinationName = pending_goto_name;
                visitedWarehouses.insert(pending_goto_name);
                setState("Moving to " + pending_goto_name);
                pending_goto = false;
                break;
            }
        }
        if (pending_goto) {
            std::cerr << "Warehouse \"" << pending_goto_name << "\" not found.\n";
            pending_goto = false;
        }
    }
    if (!en_route || speed == 0.0 || state == "Stopped") {
        this->stop();
        return;
    }
    double radians = to_radians(angle);
    double dy = (speed / 100.0) * std::cos(radians);
    double dx = (speed / 100.0) * std::sin(radians);
    _location->x += dx;
    _location->y += dy;

    if (mode == 0 && destination) {
        // Move to position, then stop
        double remaining = Vehicle::computeDistance(_location->x, _location->y,
                                                    destination->x, destination->y);
        if (remaining < 1 && !arrived) {
            _location->x = destination->x;
            _location->y = destination->y;
            setState("Stopped");
            speed = 0.0;
            en_route = false;
            arrived = true;
        } else {
            setState("Moving to position");
        }
    } else if (mode == 1) {
        // Course mode: move forever
        setState("Moving to " + std::to_string(angle));
    } else if (mode == 2 && destination) {
        // Patrol/warehouse mode
        double remaining = Vehicle::computeDistance(_location->x, _location->y,
                                                    destination->x, destination->y);
        if (remaining < 1 && !arrived) {
            _location->x = destination->x;
            _location->y = destination->y;
            setState("Arrived at " + currentDestinationName);
            en_route = false;
            arrived = true;
            // Start patrol to next warehouse (unless all visited)
            startPatrol();
        }
    }
}

void State_trooper::broadcast_current_state() {
    std::cout << "State_trooper " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";
    if (mode == 2 && en_route) {
        // Scheduled route or manual move in progress
            std::cout << "Heading to " << currentDestinationName << ",speed: " << speed << " km/h" << std::endl;

    }
    else if (mode == 1 && en_route) {
        std::cout << "Moving on course " << std::fmod(angle, 360.0) << " deg, speed: " << speed  << " km/h" << std::endl;
    }
    else if (mode == 0 && en_route) {
        std::cout << "Moving to position (" << destination->x << ", " << destination->y << "), speed: " << speed << " km/h" << std::endl;
    }
    else {
        std::cout << this->state << "." << std::endl;
    }

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
        setState("Heading to " + nextName);
    } else {
        // All warehouses visited
        destination = nullptr;
        speed = 0.0;
        setState("Stopped");
    }
}
