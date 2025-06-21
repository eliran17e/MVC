//
// Created by User on 20/06/2025.
//

#include "Truck.h"


void Truck::update() {
    double currentTime = Model::getInstance().getTime();

    if (!en_route && !deliveryRoute.empty()) {
        DeliveryLeg& next_leg = deliveryRoute.front();

        if (currentTime >= next_leg.departureTime) {
            // Begin route
            current_leg = next_leg;

            double dx = current_leg.destinationLocation->x - current_leg.sourceLocation->x;
            double dy = current_leg.destinationLocation->y - current_leg.sourceLocation->y;
            double distance = std::sqrt(dx * dx + dy * dy);
            double duration = current_leg.arrivalTime - current_leg.departureTime;

            if (duration <= 0.0 && firstRoute == false) {
                std::cerr << "Invalid delivery leg timing.\n";
                return;
            }

            this->speed = distance / duration;
            this->angle = to_degrees(std::atan2(dx, dy));
            this->destination = current_leg.destinationLocation;
            setState("Moving to " + current_leg.destinationName);
            en_route = true;
            arrived = false;
            firstRoute = false;
        }
    }

    if (en_route && this->destination) {
        // Move
        double radians = to_radians(this->angle);
        double dx = this->speed * std::sin(radians);
        double dy = this->speed * std::cos(radians);

        _location->x += dx;
        _location->y += dy;

        // Check arrival
        double remaining = Vehicle::computeDistance(_location->x, _location->y,
                                                    this->destination->x, this->destination->y);

        if (remaining < 0.1 && !arrived) {
            // Snap to exact destination
            _location = this->destination;

            // Unload
            num_boxes -= current_leg.crates;
            if (num_boxes < 0) num_boxes = 0;

            // You could notify the warehouse here to increase inventory

            setState("Parked at " + current_leg.destinationName);
            arrived = true;
        }

        // Done with leg after arrival and passed arrival time
        if (arrived && currentTime >= current_leg.arrivalTime) {
            deliveryRoute.pop();
            this->destination = nullptr;
            en_route = false;
            this->speed = 0.0;
        }
    }
}

void Truck::addDeliveryLeg(const std::string& destinationName,
                           std::shared_ptr<Point> sourceLocation,
                           std::shared_ptr<Point> destinationLocation,
                           double arrivalTime,
                           int crates,
                           double departureTime) {
    DeliveryLeg leg;
    leg.destinationName = destinationName;
    leg.sourceLocation = sourceLocation;
    leg.destinationLocation = destinationLocation;
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
    this->speed = speed;
}

void Truck::broadcast_current_state() {
    std::cout << "Truck " << getName()
              << " at (" << std::fixed << std::setprecision(2)
              << _location->x << ", " << _location->y << "), ";

    if (destination && !arrived) {
        // Scheduled route or manual move in progress
        if (!current_leg.destinationName.empty()) {
            std::cout << "Heading to " << current_leg.destinationName << ", ";
        } else {
            std::cout << "Heading to destination, ";
        }
    } else {
        std::cout << "Stopped, ";
    }

    std::cout << "Crates: " << num_boxes << std::endl;
}

