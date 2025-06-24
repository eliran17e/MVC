//
// Created by User on 20/06/2025.
//

#ifndef GANGBIKE_H
#define GANGBIKE_H
#include "Vehicle.h"


class Chopper :   public Vehicle {


    private:
        double attack_range;
        bool attack_success= false;
        std::string targetTruckName;

    public:
        Chopper(std::string name, std::shared_ptr<Point> location, std::string state = "stopped", double attack_range = 0.02)
            : Vehicle(std::move(name), std::move(location), std::move(state)), attack_range(attack_range) {}

        ~Chopper() override = default;

        void update() override;

        void course(double angle, double speed) override;
        void position(double x, double y, double speed) override;
        void broadcast_current_state() override ;
        bool attack(const std::string& truckName); // 🚨 now takes truck name
    };



#endif //GANGBIKE_H
