#ifndef STATE_TROOPER_H
#define STATE_TROOPER_H

#include <set>

#include "Vehicle.h"
#include "Model.h" // For access to warehouses

class State_trooper : public Vehicle {

private:
    bool en_route = false;
    bool arrived = false;
    std::string currentDestinationName;
    set<std::string> visitedWarehouses;
    void startPatrol();



public:
    State_trooper(const std::string& name, std::shared_ptr<Point> location, const std::string& state = "stopped")
        : Vehicle(name, std::move(location), state) {}

    void update() override;
    void broadcast_current_state() override;

    void course(double angle,double speed ) override;

    void position(double x, double y,double speed) override;

    void goToDestination(const std::string& warehouseName);
};

#endif // STATE_TROOPER_H
