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
class Sim_obj {
private:
    std::string name;


public:
    explicit Sim_obj(std::string n) : name(std::move(n)) {}
    virtual ~Sim_obj() = default;


    // Pure virtual methods for the derived classes
    virtual const shared_ptr<Point> &get_location() const = 0;
    virtual void update() = 0;
    virtual void broadcast_current_state() = 0;

    // Getter
    virtual const std::string& getName() const {
        return name;
    }

};




#endif //OBJECT_H
