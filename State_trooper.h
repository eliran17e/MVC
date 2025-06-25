#ifndef STATE_TROOPER_H
#define STATE_TROOPER_H

#include <set>
#include "Vehicle.h"
#include "Model.h" // For access to warehouses



/**
 * @class State_trooper
 * @brief Represents a police vehicle that can patrol between warehouses.
 *
 * Inherits from Vehicle. Capable of moving to specific warehouses and automatically patrolling
 * to unvisited warehouses when idle.
 */
class State_trooper : public Vehicle {

private:
    bool arrived = false;
    std::string currentDestinationName;
    set<std::string> visitedWarehouses;
    void startPatrol();                 ///< Initiate patrol to next unvisited warehouse

    // Pending goto logic (used for asynchronous command execution)
    bool pending_goto = false;
    std::string pending_goto_name;



public:

    /**
     * @brief Constructs a State_trooper object.
     * @param name Vehicle's unique name
     * @param location Shared pointer to starting location
     * @param state Optional initial state (default: "stopped")
     */
    State_trooper(const std::string& name, std::shared_ptr<Point> location, const std::string& state = "stopped")
        : Vehicle(name, std::move(location), state) {}


    /**
     * @brief Updates trooper's state and position each simulation tick.
     */
    void update() override;


    /**
     * @brief Prints or broadcasts the current state for UI or debug.
     */
    void broadcast_current_state() override;


    /**
     * @brief Sets a new destination warehouse for the trooper.
     * @param warehouseName Name of the warehouse to move to
     */
    void goToDestination(const std::string& warehouseName);
};

#endif // STATE_TROOPER_H
