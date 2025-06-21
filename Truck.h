//
// Created by User on 20/06/2025.
//

#ifndef TRUCK_H
#define TRUCK_H

#include <queue>

#include "Vehicle.h"
#include "memory"
#include "map"
#include "Model.h"


struct DeliveryLeg {
    std::string destinationName;
    std::shared_ptr<Point> sourceLocation;
    std::shared_ptr<Point> destinationLocation;
    double arrivalTime;  // in hours (e.g. 10.5)
    int crates;
    double departureTime; // in hours (e.g. 11.25)
};




class Truck : public Vehicle {
private:
    int num_boxes;
    std::map<std::string, int> city_boxes;
    std::queue<DeliveryLeg> deliveryRoute;
    bool en_route = false;
    bool arrived = false;
    DeliveryLeg current_leg;

    bool firstRoute = true; // Flag to indicate if this is the first route

public:
    Truck(std::string name, shared_ptr<Point> location, std::string state = "stopped",int boxes = 0)
        : Vehicle(std::move(name), std::move(location), std::move(state)), num_boxes(boxes), current_leg() {
        if (num_boxes < 0) {
            num_boxes = 0;
        }
    };

    ~Truck() override = default ;

    void update() override;

    void broadcast_current_state() override;


    void course(double angle, double speed) override;


    void position(double x, double y, double speed) override ;

    void addDeliveryLeg(const std::string& destinationName,
                        std::shared_ptr<Point> sourceLocation,
                        std::shared_ptr<Point> destinationLocation,
                        double arrivalTime,
                        int crates,
                        double departureTime);

    void popDeliveryLeg();

    void clearBoxes();

    void setBoxes(int total_crates);

    void stop () override;
};



#endif //TRUCK_H
