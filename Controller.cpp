#include "Controller.h"
#include "Model.h"
#include "Truck.h"
#include "State_trooper.h"
#include "Chopper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

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

    std::istringstream iss(line);
    std::string initialWarehouse, initialDepartureTime;
    std::getline(iss, initialWarehouse, ',');
    std::getline(iss, initialDepartureTime);

    auto& model = Model::getInstance();
    auto whs = model.getWarehouses();
    auto it = std::find_if(whs.begin(), whs.end(), [&](const std::shared_ptr<Warehouse>& wh) {
        return wh->getName() == initialWarehouse;
    });

    if (it == whs.end()) {
        std::cerr << "ERROR: Warehouse " << initialWarehouse << " not found for truck " << truckName << "\n";
        return;
    }

    auto truck = std::make_shared<Truck>(truckName, (*it)->get_location(), "stopped", 0);
    double initialDeparture = model.parseTime(initialDepartureTime);

    std::shared_ptr<Point> currentLocation = (*it)->get_location();  // Start at initial warehouse
    std::string currentWarehouse = initialWarehouse;

    // Add initial "dummy" leg to initialize time
    truck->addDeliveryLeg(currentWarehouse, currentLocation, currentLocation, initialDeparture, 0, initialDeparture);

    int totalCrates = 0;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string warehouse, arrivalStr, boxesStr, departureStr;
        std::getline(lineStream, warehouse, ',');
        std::getline(lineStream, arrivalStr, ',');
        std::getline(lineStream, boxesStr, ',');
        std::getline(lineStream, departureStr);

        auto w_it = std::find_if(whs.begin(), whs.end(), [&](const std::shared_ptr<Warehouse>& wh) {
            return wh->getName() == warehouse;
        });

        if (w_it == whs.end()) {
            std::cerr << "ERROR: Warehouse " << warehouse << " not found in schedule for truck " << truckName << "\n";
            continue;
        }

        double arrival = model.parseTime(arrivalStr);
        double departure = model.parseTime(departureStr);
        int crates = std::stoi(boxesStr);

        totalCrates += crates;

        // Add leg from previous warehouse/location to current one
        truck->addDeliveryLeg(
            warehouse,
            currentLocation,                // source location
            (*w_it)->get_location(),        // destination location
            arrival,
            crates,
            departure
        );

        // Update for next leg
        currentLocation = (*w_it)->get_location();
        currentWarehouse = warehouse;
    }

    truck->setBoxes(totalCrates);
    model.addVehicle(truck);
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
                    auto trooper = std::make_shared<State_trooper>(objName, wh->get_location());
                    Model::getInstance().addVehicle(trooper);
                }
            } else if (type == "Chopper") {
                double x, y;
                if (iss >> x >> y) {
                    auto chopper = std::make_shared<Chopper>(objName, std::make_shared<Point>(x, y));
                    Model::getInstance().addVehicle(chopper);
                } else {
                    std::cerr << "ERROR: Invalid coordinates for Chopper creation.\n";
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
