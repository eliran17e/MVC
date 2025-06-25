//
// Created by User on 20/06/2025.
//

#include "Truck.h"

#include <utility>


void Truck::update() {
    if (pending_position) {
        // Clear any routes, set mode, state, etc.
        while (!deliveryRoute.empty()) deliveryRoute.pop();

        this->destination = std::make_shared<Point>(pending_pos_x, pending_pos_y);
        double dx = pending_pos_x - _location->x;
        double dy = pending_pos_y - _location->y;
        this->angle = to_degrees(std::atan2(dx, dy));
        this->speed = pending_pos_speed / 100.0;
        this->mode = 0;
        this->en_route = true;
        this->arrived = false;
        setState("Moving to manual destination");

        pending_position = false;
        // Do NOT return here: continue to rest of logic!
    }

    if (pending_course) {
        while (!deliveryRoute.empty()) deliveryRoute.pop();
        this->angle = pending_course_angle;
        this->speed = pending_course_speed / 100.0;
        this->destination = nullptr;
        this->mode = 1;
        this->en_route = true;
        this->arrived = false;
        setState("Moving on course");

        pending_course = false;
        // Do NOT return here: continue to rest of logic!
    }

    double currentTime = Model::getInstance().getTime();
    if (firstRoute && !deliveryRoute.empty()) {
        // If this is the first route, set the first leg
        Model::getInstance().findWarehouseByName(startingWarehouseName)->update_boxes(-num_boxes);
        firstRoute = false;
        Model::getInstance().findWarehouseByName(startingWarehouseName)->update();

    }

    // MODE 0: Move to position, stop at destination
    if (mode == 0 && en_route && destination) {
        double radians = to_radians(angle);
        double dy = (speed ) * std::cos(radians);
        double dx = (speed ) * std::sin(radians);

        _location->x += dx;
        _location->y += dy;

        double remaining = Vehicle::computeDistance(_location->x, _location->y, destination->x, destination->y);
        if (remaining < speed && !arrived) {
            _location->x = destination->x;
            _location->y = destination->y;
            setState("Stopped");
            speed = 0.0;
            en_route = false;
            arrived = true;
        }
        return; // Don't continue to delivery logic
    }


    if (mode == 1 && en_route) {
        double radians = to_radians(angle);
        double dy = (speed ) * std::cos(radians);
        double dx = (speed ) * std::sin(radians);

        _location->x += dx;
        _location->y += dy;

        setState("Moving on course");
        return; // Don't continue to delivery logic
    }

    // MODE 2: Normal delivery route
    if (!en_route && !deliveryRoute.empty()) {
        // Remove any dummy legs or bad data
        while (!deliveryRoute.empty()) {
            DeliveryLeg& next_leg = deliveryRoute.front();
            double dx = next_leg.destinationLocation->x - next_leg.sourceLocation->x;
            double dy = next_leg.destinationLocation->y - next_leg.sourceLocation->y;
            double distance = std::sqrt(dx * dx + dy * dy);
            double duration = next_leg.arrivalTime - next_leg.departureTime;

            if (distance < 0.001 || duration <= 0.0) {
                deliveryRoute.pop();
            } else {
                break; // found real leg
            }
        }

        if (!deliveryRoute.empty()) {
            DeliveryLeg& leg = deliveryRoute.front();
            if (currentTime >= leg.departureTime) {
                current_leg = leg;
                double dx = leg.destinationLocation->x - leg.sourceLocation->x;
                double dy = leg.destinationLocation->y - leg.sourceLocation->y;
                double distance = std::sqrt(dx * dx + dy * dy);
                double duration = leg.arrivalTime - leg.departureTime;

                speed = distance / duration;
                angle = to_degrees(std::atan2(dy, dx));
                destination = leg.destinationLocation;
                setState("Moving to " + leg.destinationName);
                en_route = true;
                arrived = false;
            }
        }
    }

    if (en_route && destination && !arrived && mode == 2) {
        double radians = to_radians(angle);
        double dx = speed * std::cos(radians);
        double dy = speed * std::sin(radians);

        _location->x += dx;
        _location->y += dy;

        double remaining = Vehicle::computeDistance(_location->x, _location->y, destination->x, destination->y);
        double time_to_arrival = current_leg.arrivalTime - currentTime;

        if (remaining < 0.1 || time_to_arrival <= 1.0) {
            _location->x = destination->x;
            _location->y = destination->y;

            if (!unloaded) {  // Only unload once per stop
                num_boxes -= current_leg.crates;
                if (num_boxes < 0) num_boxes = 0;
                unloaded = true;
                Model::getInstance().findWarehouseByName(current_leg.destinationName)->update_boxes(current_leg.crates);
                Model::getInstance().findWarehouseByName(current_leg.destinationName)->update();
            }

            setState("Parked at " + current_leg.destinationName);
            arrived = true;
            en_route = false;
        }
    }

    // Remove leg if it's complete and time passed
    if (arrived && currentTime >= current_leg.arrivalTime && mode == 2) {
        deliveryRoute.pop();
        destination = nullptr;
        arrived = false;
        speed = 0.0;
        unloaded = false;

        if (deliveryRoute.empty()) {
            setState("Parked at " + current_leg.destinationName);
        }
    }
}


void Truck::addDeliveryLeg(const std::string& destinationName,
                           std::shared_ptr<Point> sourceLocation,
                           std::shared_ptr<Point> destinationLocation,
                           double departureTime,
                           int crates,
                           double arrivalTime) {
    DeliveryLeg leg;
    leg.destinationName = destinationName;
    leg.sourceLocation = std::move(sourceLocation);
    leg.destinationLocation = std::move(destinationLocation);
    leg.arrivalTime = arrivalTime;
    leg.crates = crates;
    leg.departureTime = departureTime;
    deliveryRoute.push(leg);
}

void Truck::popDeliveryLeg() {
    if (!deliveryRoute.empty()) {
        deliveryRoute.pop();
    }

}

void Truck::clearBoxes() {

    this->num_boxes = 0;
}

void Truck::setBoxes(int total_crates) {
    num_boxes = total_crates;
}

void Truck::stop() {
    Vehicle::stop();
    en_route = false;
    arrived = false;
    while (!deliveryRoute.empty()) deliveryRoute.pop();
}


void Truck::broadcast_current_state() {
    std::cout << "Truck " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (mode == 2 && en_route && !arrived) {
        // Scheduled route or manual move in progress
        if (!current_leg.destinationName.empty()) {
            std::cout << "Heading to " << current_leg.destinationName << ",speed: " << speed * 100 << " km/h, ";
        }
    }
    else if (mode == 1 && en_route) {
        std::cout << "Moving on course " << std::fmod(angle, 360.0) << " deg, speed: " << speed * 100 << " km/h, ";
    }
    else if (mode == 0 && en_route) {
        std::cout << "Moving to position (" << destination->x << ", " << destination->y << "), speed: " << speed * 100 << " km/h, ";
    }
    else {
        std::cout << this->state << ", ";
    }

    std::cout << "Crates: " << num_boxes << std::endl;
}

