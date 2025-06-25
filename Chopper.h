//
// Created by User on 20/06/2025.
//

#ifndef GANGBIKE_H
#define GANGBIKE_H
#include "Vehicle.h"

/**
 * @class Chopper
 * @brief Represents a chopper vehicle in the simulation that can move and attack trucks.
 *
 * The Chopper can:
 *  - Move to a specified position (position mode)
 *  - Move indefinitely on a course (angle) (course mode)
 *  - Attempt to attack trucks (attack mode)
 * Inherits from Vehicle, which in turn inherits from Sim_obj.
 */
class Chopper :   public Vehicle {

private:
        double attack_range; ///< Attack range in km (default: 2 km, min: 1 km, max: 20 km)
        bool attack_success= false;
        std::string targetTruckName;
        bool pending_attack = false;
        std::string pending_attack_target;

public:
        /**
     * @brief Constructs a Chopper.
     * @param name           The chopper's name.
     * @param location       Starting location as a shared pointer to a Point.
     * @param state          Initial state string (default: "stopped").
     * @param attack_range   Attack range in km (default: 2.0 km).
     */
    Chopper(std::string name, std::shared_ptr<Point> location, std::string state = "stopped", double attack_range = 0.02)
    : Vehicle(std::move(name), std::move(location), std::move(state)), attack_range(attack_range) {}

    ~Chopper() override = default;

    /**
     * @brief Updates the chopper's location, state, and attack logic.
     *
     * Moves or attacks according to current mode:
     *  - Position mode: Move toward target position, stop when arrived.
     *  - Course mode: Move at constant angle and speed.
     *  - Attack mode: Attempt to attack a truck within range.
     */
    void update() override;
    /**
    * @brief Prints the current state and location of the chopper to stdout.
    */
    void broadcast_current_state() override ;
    /**
     * @brief Attempts to attack a truck by name.
     * @param truckName Name of the truck to attack.
     * @return true if attack succeeds, false otherwise.
     *
     * Attack will only succeed if:
     *  - The truck exists and is within attack_range.
     *  - No State_trooper is within 10 km of the truck.
     * On success, disables the truck and increases attack_range (up to 20 km).
     * On failure, decreases attack_range (down to 1 km).
     * On both success and failure, sets the state to "Stopped".
     */

    bool attack(const std::string& truckName);
    };



#endif //GANGBIKE_H
