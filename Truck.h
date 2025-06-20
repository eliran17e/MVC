//
// Created by User on 20/06/2025.
//

#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"
#include "Geometry.h"
#include "memory"
class Truck : public Vehicle {
private:
    std::shared_ptr<Point> _location;
    int num_boxes;
public:
    Truck(std::string name, std::string details) {};
    ~Truck() ;

};



#endif //TRUCK_H
