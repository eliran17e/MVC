//
// Created by User on 20/06/2025.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <string>
#include <utility>
#include "Geometry.h"
using std::shared_ptr;


/**
 * @class Sim_obj
 * @brief Abstract base class for all simulation objects (vehicles, warehouses, etc.).
 *
 * Provides a common interface for simulation objects, including unique naming,
 * position retrieval, state updates, and status broadcasting.
 */
class Sim_obj {
private:
    std::string name;


public:
    /**
     * @brief Construct a Sim_obj with a given name.
     * @param n Name of the simulation object.
     */
    explicit Sim_obj(std::string n) : name(std::move(n)) {}


    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~Sim_obj() = default;


    /**
     * @brief Get the current location of the simulation object.
     * @return Shared pointer to the object's position.
     */
    virtual const shared_ptr<Point> &get_location() const = 0;


    /**
    * @brief Update the object's state for the next simulation tick.
    */
    virtual void update() = 0;


    /**
    * @brief Print or broadcast the current state of the object.
    */
    virtual void broadcast_current_state() = 0;

    /**
     * @brief Get the name of the simulation object.
     * @return Reference to the object's name string.
     */
    virtual const std::string& getName() const {
        return name;
    }

};




#endif //OBJECT_H
