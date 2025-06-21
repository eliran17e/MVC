//
// Created by User on 20/06/2025.
//

#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>

#include "Sim_obj.h"
#include "math.h"


class Vehicle :   public Sim_obj {
protected:
    std::string state;
    shared_ptr<Point> _location;

    double angle;                       // in degrees (0-360)
    double speed;                       // in km/h
    shared_ptr<Point> destination;      // optional, only when moving to a location

public:
    Vehicle();


    Vehicle(std::string name, shared_ptr<Point> location, std::string state = "stopped")
        : Sim_obj(std::move(name)), state(std::move(state)),
    _location(std::move(location)), angle(0.0), speed(0.0), destination(nullptr) {}


    ~Vehicle() override=default;
    const shared_ptr<Point> &get_location() const override;
    void set_location(const shared_ptr<Point> &location) ;
    void update() override = 0;

    void broadcast_current_state() override = 0;

    virtual void course(double angle, double speed) = 0;

    virtual void position(double x, double y, double speed) = 0;


    const std::string &getState() const {
        return state;
    }

    void setState(const string &state1) {
        state = state1;
    }

    virtual void stop();

    static double computeDistance(double x1, double y1, double x2, double y2);

    void setSpeed(double x);

};



#endif //VEHICLE_H
