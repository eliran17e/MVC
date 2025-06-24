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


struct DeliveryLeg {
    std::string destinationName;
    std::shared_ptr<Point> sourceLocation;
    std::shared_ptr<Point> destinationLocation;
    double departureTime; // in hours (e.g. 11.25)
    int crates;
    double arrivalTime;  // in hours (e.g. 10.5)
};




class Truck : public Vehicle {
private:
    int num_boxes;
    std::map<std::string, int> city_boxes;
    std::queue<DeliveryLeg> deliveryRoute;
    bool en_route = false;
    bool arrived = false;
    DeliveryLeg current_leg;
    bool unloaded = false;

public:
    Truck(std::string name, std::shared_ptr<Point> location, std::string state = "stopped", int boxes = 0)
: Vehicle(std::move(name), location, std::move(state)),  // Don't move shared_ptr!
      num_boxes(boxes),
      current_leg()
    {
        this->mode = 2;

        if (num_boxes < 0) {
            num_boxes = 0;
        }
    }

    ~Truck() override = default ;

    void update() override;

    void broadcast_current_state() override;


    void course(double angle, double speed) override;


    void position(double x, double y, double speed) override ;

    void addDeliveryLeg(const std::string& destinationName,
                           std::shared_ptr<Point> sourceLocation,
                           std::shared_ptr<Point> destinationLocation,
                           double departureTime,
                           int crates,
                           double arrivalTime);

    void popDeliveryLeg();

    void clearBoxes();

    void setBoxes(int total_crates);

    void stop () override;
    std::queue<DeliveryLeg> getDeliveryQueue() const {
        return deliveryRoute;
    }
};



#endif //TRUCK_H
