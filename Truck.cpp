//
// Created by User on 20/06/2025.
//

#include "Truck.h"

#include <utility>


void Truck::update() {
    double currentTime = Model::getInstance().getTime();

    // MODE 0: Move to position, stop at destination
    if (mode == 0 && en_route && destination) {
        double radians = to_radians(angle);
        double dy = (speed / 100.0) * std::cos(radians);
        double dx = (speed / 100.0) * std::sin(radians);

        _location->x += dx;
        _location->y += dy;

        double remaining = Vehicle::computeDistance(_location->x, _location->y, destination->x, destination->y);
        if (remaining < speed/100 && !arrived) {
            _location->x = destination->x;
            _location->y = destination->y;
            setState("Stopped at manual position");
            speed = 0.0;
            en_route = false;
            arrived = true;
        }
        return; // Don't continue to delivery logic
    }

    // MODE 1: Move at angle forever, never stop
    if (mode == 1 && en_route) {
        double radians = to_radians(angle);
        double dy = (speed / 100.0) * std::cos(radians);
        double dx = (speed / 100.0) * std::sin(radians);

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
            setState("All deliveries complete");
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



void Truck::position(double x, double y, double speed) {
    // Clear current route
    while (!deliveryRoute.empty()) deliveryRoute.pop();

    this->destination = std::make_shared<Point>(x, y);

    // Calculate angle and distance
    double dx = this->destination->x - _location->x;
    double dy = this->destination->y - _location->y;

    this->angle = to_degrees(std::atan2(dx, dy));

    en_route = true;
    arrived = false;
    this->mode = 0; // position mode
    setState("Moving to manual destination");
    this->speed = speed;
}

void Truck::course(double angle, double speed) {
    // Clear current route
    while (!deliveryRoute.empty()) deliveryRoute.pop();

    this->angle = angle;
    this->speed = speed;
    this->state = "Moving on course";
    this->destination = nullptr; // course-based movement
    en_route = true;
    arrived = false;
    mode = 1;
    this->speed = speed;
}

void Truck::broadcast_current_state() {
    std::cout << "Truck " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (destination && !arrived) {
        // Scheduled route or manual move in progress
        if (!current_leg.destinationName.empty()) {
            std::cout << "Heading to " << current_leg.destinationName << ",speed: " << speed * 100 << " km/h, ";
        } else {
            std::cout << "Heading to destination, ";
        }
    } else {
        std::cout << "Stopped, ";
    }

    std::cout << "Crates: " << num_boxes << std::endl;
}

