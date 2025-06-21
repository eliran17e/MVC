//
// Created by User on 20/06/2025.
//

#ifndef GANGBIKE_H
#define GANGBIKE_H
#include "Vehicle.h"


class Chopper :   public Vehicle {


    private:
        int attack_range;

    public:
        Chopper(std::string name, std::shared_ptr<Point> location, std::string state = "stopped", int attack_range = 2)
            : Vehicle(std::move(name), std::move(location), std::move(state)), attack_range(attack_range) {}

        ~Chopper() override = default;

        void update() override;

        void course(double angle, double speed) override;
        void position(double x, double y, double speed) override;
        void broadcast_current_state() {
        // Your logic or leave empty for now
    }
        bool attack(const std::string& truckName); // 🚨 now takes truck name
    };



#endif //GANGBIKE_H
