cmake_minimum_required(VERSION 3.31)
project(simDepot)

set(CMAKE_CXX_STANDARD 11)

add_executable(simDepot main.cpp
        Truck.cpp
        Truck.h
        State_trooper.cpp
        State_trooper.h
        Chopper.cpp
        Chopper.h
        Sim_obj.cpp
        Sim_obj.h
        Geometry.cpp
        Geometry.h
        Vehicle.cpp
        Vehicle.h
        Controller.cpp
        Controller.h
        Model.cpp
        Model.h
        View.cpp
        View.h
        Warehouse.cpp
        Warehouse.h
        MyExceptions.h)
