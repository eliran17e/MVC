#include "Controller.h"
#include "Model.h"
#include "Truck.h"
#include "State_trooper.h"
#include "Chopper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
Controller::Controller() {
    Model::getInstance();
}
void Controller::run(int argc, char* argv[]) {
    std::string warehouseFile;
    std::vector<std::string> truckFiles;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-w" && i + 1 < argc) {
            warehouseFile = argv[++i];
        } else if (arg == "-t") {
            while (++i < argc) {
                truckFiles.push_back(argv[i]);
            }
        }
    }

    if (warehouseFile.empty()) {
        std::cerr << "ERROR: Warehouse file not specified with -w\n";
        return;
    }

    loadWarehouses(warehouseFile);
    for (const auto& truckFile : truckFiles) {
        loadTruckSchedule(truckFile);
    }

    commandLoop();
}

void Controller::loadWarehouses(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: Cannot open warehouse file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string name;
        double x, y;
        int crates;

        size_t name_end = line.find(',');
        if (name_end == std::string::npos) continue;
        name = line.substr(0, name_end);
        name.erase(name.find_last_not_of(" \t\r\n") + 1);

        size_t coord_start = line.find('(', name_end);
        size_t coord_end = line.find(')', coord_start);
        if (coord_start == std::string::npos || coord_end == std::string::npos) continue;

        std::string coord_str = line.substr(coord_start + 1, coord_end - coord_start - 1);
        if (sscanf(coord_str.c_str(), "%lf, %lf", &x, &y) != 2) continue;

        size_t crates_start = line.find(',', coord_end);
        if (crates_start == std::string::npos) continue;
        try {
            crates = std::stoi(line.substr(crates_start + 1));
        } catch (...) {
            continue;
        }

        Model::getInstance().addWarehouse(
            std::make_shared<Warehouse>(name, std::make_shared<Point>(x, y), crates));
    }
}


void Controller::loadTruckSchedule(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: Cannot open truck file: " << filename << "\n";
        return;
    }

    std::string truckName = filename.substr(filename.find_last_of("/\\") + 1);
    if (truckName.find('.') != std::string::npos) {
        truckName = truckName.substr(0, truckName.find_last_of('.'));
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "ERROR: Truck file empty: " << filename << "\n";
        return;
    }

    auto& model = Model::getInstance();
    auto whs = model.getWarehouses();

    // Parse the initial line for start location and departure
    std::istringstream iss(line);
    std::string prevWarehouseName, prevDepartureTimeStr;
    std::getline(iss, prevWarehouseName, ',');
    std::getline(iss, prevDepartureTimeStr);
    auto prevWarehouseIt = std::find_if(whs.begin(), whs.end(), [&](const std::shared_ptr<Warehouse>& wh) {
        return wh->getName() == prevWarehouseName;
    });
    if (prevWarehouseIt == whs.end()) {
        std::cerr << "ERROR: Warehouse " << prevWarehouseName << " not found for truck " << truckName << "\n";
        return;
    }
    std::shared_ptr<Point> prevLocation = (*prevWarehouseIt)->get_location();
    double prevDepartureTime = model.parseTime(prevDepartureTimeStr);

    auto truck = std::make_shared<Truck>(
        truckName,
        std::make_shared<Point>(*prevLocation),
        "stopped",
        0
    );
    int totalCrates = 0;

    // Now parse each delivery leg
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string warehouseName, arrivalStr, cratesStr, departureStr;
        std::getline(lineStream, warehouseName, ',');
        std::getline(lineStream, arrivalStr, ',');
        std::getline(lineStream, cratesStr, ',');
        std::getline(lineStream, departureStr);

        auto warehouseIt = std::find_if(whs.begin(), whs.end(), [&](const std::shared_ptr<Warehouse>& wh) {
            return wh->getName() == warehouseName;
        });
        if (warehouseIt == whs.end()) {
            std::cerr << "ERROR: Warehouse " << warehouseName << " not found in schedule for truck " << truckName << "\n";
            continue;
        }
        std::shared_ptr<Point> warehouseLoc = (*warehouseIt)->get_location();
        double arrivalTime = model.parseTime(arrivalStr);
        double departureTime = model.parseTime(departureStr);
        int crates = std::stoi(cratesStr);

        totalCrates += crates;

        // Build the leg from prevWarehouse to this warehouse
        truck->addDeliveryLeg(
            warehouseName,        // destination name
            prevLocation,         // sourceLocation
            warehouseLoc,         // destinationLocation
            prevDepartureTime,    // depart (from previous warehouse)
            crates,
            arrivalTime           // arrive (to current warehouse)
        );

        // Update for next leg
        prevLocation = warehouseLoc;
        prevDepartureTime = departureTime;
    }

    truck->setBoxes(totalCrates);
    model.addVehicle(truck);



    std::queue<DeliveryLeg> tmp = truck->getDeliveryQueue();
    while (!tmp.empty()) {
        const auto& leg = tmp.front();

        tmp.pop();
    }
}


void Controller::commandLoop() {
    std::string line;
    std::cout << "> ";

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "exit") break;

        if (command == "status") {
            Model::getInstance().status();
        } else if (command == "go") {
            Model::getInstance().go();
        } else if (command == "default") {
            Model::getInstance().getView()->default_size();
        } else if (command == "size") {
            int s;
            if (iss >> s && s >= 6 && s <= 30) {
                Model::getInstance().getView()->new_size(s);
            } else {
                std::cerr << "ERROR: Invalid size. Must be between 6 and 30.\n";
            }
        } else if (command == "zoom") {
            double z;
            if (iss >> z && z > 0) {
                Model::getInstance().getView()->new_zoom(z);
            } else {
                std::cerr << "ERROR: Invalid zoom. Must be a positive number.\n";
            }
        } else if (command == "pan") {
            double x, y;
            if (iss >> x >> y) {
                Model::getInstance().getView()->pan(x, y);
            } else {
                std::cerr << "ERROR: Invalid pan coordinates.\n";
            }
        } else if (command == "show") {
            Model::getInstance().getView()->clear();
            Model::getInstance().insert_objects();
            Model::getInstance().getView()->show();
        } else if (command == "create") {
            std::string objName, type;
            iss >> objName >> type;
            if (type == "State_trooper") {
                std::string warehouseName;
                iss >> warehouseName;
                auto wh = Model::getInstance().findWarehouseByName(warehouseName);
                if (!wh) {
                    std::cerr << "ERROR: Warehouse '" << warehouseName << "' not found.\n";
                } else {
                    auto trooper = std::make_shared<State_trooper>(objName, std::make_shared<Point>(wh->get_location()->x, wh->get_location()->y));
                    Model::getInstance().addVehicle(trooper);
                }
            } else if (type == "Chopper") {
                std::string coord;
                std::getline(iss, coord); // grab the rest of the line (e.g. "(14.00, 14.00)")

                double x, y;
                if (sscanf(coord.c_str(), " (%lf , %lf)", &x, &y) == 2) {
                    auto chopper = std::make_shared<Chopper>(objName, std::make_shared<Point>(x, y));
                    Model::getInstance().addVehicle(chopper);
                } else {
                    std::cerr << "ERROR: Invalid coordinates format for Chopper. Use (x, y)\n";
                }
            } else {
                std::cerr << "ERROR: Invalid type. Only 'State_trooper' and 'Chopper' are supported.\n";
            }
        } else {
            std::string objName = command;
            auto vehicle = Model::getInstance().findVehicleByName(objName);
            if (!vehicle) {
                std::cerr << "ERROR: Unknown object '" << objName << "'\n";
                std::cout << "> ";
                continue;
            }

            std::string action;
            iss >> action;

            if (action == "course") {
                double angle;
                iss >> angle;
                if (std::dynamic_pointer_cast<Chopper>(vehicle)) {
                    double speed;
                    if (iss >> speed) {
                        if (speed > 170.0) {
                            std::cerr << "ERROR: Chopper speed cannot exceed 170 km/h.\n";
                        } else {
                            vehicle->course(angle, speed);
                        }
                    } else {
                        std::cerr << "ERROR: Chopper requires speed in 'course' command.\n";
                    }
                } else {
                    vehicle->course(angle,90); // Truck or Trooper
                }
            } else if (action == "position") {
                double x, y;
                iss >> x >> y;
                if (std::dynamic_pointer_cast<Chopper>(vehicle)) {
                    double speed;
                    if (iss >> speed) {
                        if (speed > 170.0) {
                            std::cerr << "ERROR: Chopper speed cannot exceed 170 km/h.\n";
                        } else {
                            vehicle->position(x, y, speed);
                        }
                    } else {
                        std::cerr << "ERROR: Chopper requires speed in 'position' command.\n";
                    }
                } else {
                    vehicle->position(x, y,90);
                }
            } else if (action == "destination") {
                auto trooper = std::dynamic_pointer_cast<State_trooper>(vehicle);
                if (trooper) {
                    std::string warehouseName;
                    iss >> warehouseName;
                    trooper->goToDestination(warehouseName);
                } else {
                    std::cerr << "ERROR: Only State_trooper supports 'destination' command.\n";
                }
            } else if (action == "attack") {
                auto chopper = std::dynamic_pointer_cast<Chopper>(vehicle);
                if (chopper) {
                    std::string target;
                    iss >> target;
                    chopper->attack(target);
                } else {
                    std::cerr << "ERROR: Only Chopper supports 'attack' command.\n";
                }
            } else if (action == "stop") {
                vehicle->stop();
            } else {
                std::cerr << "ERROR: Unknown command '" << action << "' for object '" << objName << "'\n";
            }
        }

        std::cout << "> ";
    }
}
