//
// Created by User on 20/06/2025.
//

#ifndef TRUCK_H
#define TRUCK_H

#include <queue>
#include <utility>
#include "Vehicle.h"
#include "memory"
#include "map"
#include "Model.h"




/**
 * @struct DeliveryLeg
 * @brief Represents a single segment ("leg") of a truck's delivery schedule.
 *
 * Contains information about the source, destination, departure/arrival times, and number of crates.
 */
struct DeliveryLeg {
    std::string destinationName;
    std::shared_ptr<Point> sourceLocation;
    std::shared_ptr<Point> destinationLocation;
    double departureTime; // in hours (e.g. 11.25)
    int crates;
    double arrivalTime;  // in hours (e.g. 10.5)

};



/**
 * @class Truck
 * @brief Simulates a delivery truck that follows a route between warehouses.
 *
 * Inherits from Vehicle. The truck can follow a pre-loaded delivery route, move by course or to a manual position,
 * and updates inventory at each stop. Also supports user-initiated movement and crate unloading.
 */
class Truck : public Vehicle {
private:
    std::string startingWarehouseName; // Name of the starting warehouse
    int num_boxes;
    std::map<std::string, int> city_boxes;
    std::queue<DeliveryLeg> deliveryRoute;
    bool arrived = false;
    DeliveryLeg current_leg;
    bool unloaded = false;
    bool firstRoute; // true if this is the first leg of the route

public:

    /**
     * @brief Constructor for Truck.
     * @param name Truck name
     * @param location Initial location
     * @param state Initial state (default: "stopped")
     * @param boxes Initial crate count (default: 0)
     * @param firstWareHouseName Name of first warehouse (optional)
     */
    Truck(std::string name, std::shared_ptr<Point> location, std::string state = "stopped", int boxes = 0,std::string firstWareHouseName = "")
: Vehicle(std::move(name), location, std::move(state)),
      num_boxes(boxes),
      current_leg()
    {
        this->mode = 2;
        firstRoute = true;
        this->startingWarehouseName = std::move(firstWareHouseName);

        if (num_boxes < 0) {
            num_boxes = 0;
        }
    }

    ~Truck() override = default ;
    /**
     * @brief Updates the truck's position, state, and handles crate delivery logic.
     */
    void update() override;


    /**
     * @brief Prints or broadcasts the truck's current state.
     */
    void broadcast_current_state() override;


    /**
     * @brief Adds a new leg to the delivery route.
     * @param destinationName Name of destination warehouse
     * @param sourceLocation Start point
     * @param destinationLocation End point
     * @param departureTime Scheduled departure time (hours)
     * @param crates Number of crates for this leg
     * @param arrivalTime Scheduled arrival time (hours)
     */
    void addDeliveryLeg(const std::string& destinationName,
                           std::shared_ptr<Point> sourceLocation,
                           std::shared_ptr<Point> destinationLocation,
                           double departureTime,
                           int crates,
                           double arrivalTime);


    /**
     * @brief Removes the current delivery leg from the route (after arrival).
     */
    void popDeliveryLeg();


    /**
     * @brief Sets the number of boxes to zero (truck was robbed/emptied).
     */
    void clearBoxes();


    /**
     * @brief Sets the total number of boxes on the truck.
     * @param total_crates The number of crates to set
     */
    void setBoxes(int total_crates);


    /**
     * @brief Stops the truck, clears the route, and sets state to stopped.
     */
    void stop () override;

    /**
     * @brief Returns a copy of the delivery queue for debugging/status display.
     * @return Delivery route queue (copy)
     */
    std::queue<DeliveryLeg> getDeliveryQueue() const {
        return deliveryRoute;
    }
};



#endif //TRUCK_H
