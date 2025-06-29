//
// Created by User on 20/06/2025.
//

#include "Vehicle.h"

void Vehicle::set_location(const shared_ptr<Point> &location) {
    _location = std::make_shared<Point>(*location);
}
const shared_ptr<Point> &Vehicle::get_location() const {
    return _location;
}

void Vehicle::stop() {
    this->angle = 0.0;
    this->speed = 0.0;
    this->destination = nullptr;
    this->en_route = false;
    setState("Stopped");
}

double Vehicle::computeDistance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void Vehicle::setSpeed(double x) {
    if (x< 0) {
        std::cerr << "Speed cannot be negative. Setting to 0." << std::endl;
        x = 0;
    }
    this->speed = x;
}
void Vehicle::position(double x, double y, double speed) {
    pending_position = true;
    pending_pos_x = x;
    pending_pos_y = y;
    pending_pos_speed = speed;
}

void Vehicle::course(double angle, double speed) {
    pending_course = true;
    pending_course_angle = angle;
    pending_course_speed = speed;
}