//
// Created by User on 20/06/2025.
//

#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>

#include "Sim_obj.h"


class Vehicle :   public Sim_obj {
private:
    std::string state;


public:
    Vehicle();
    ~Vehicle() override=default;
    const shared_ptr<Point> &get_location() const override = 0;

    void update() override = 0;

    void broadcast_current_state() override = 0;

    virtual void course(double angle) = 0;

    virtual void course(double angle, double speed) = 0;

    virtual void position(double x, double y) = 0;

    virtual void position(double x, double y, double speed) = 0;


    const std::string &getState() const {
        return state;
    }

    void setState(const string &state1) {
        state = state1;
    }



};



#endif //VEHICLE_H
