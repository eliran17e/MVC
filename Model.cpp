#include "Model.h"

#include <sstream>

void Model::addSimObj(const std::shared_ptr<Sim_obj>& obj) {
    sim_objects.push_back(obj);
}

void Model::addVehicle(const std::shared_ptr<Vehicle>& v) {
    vehicles.push_back(v);
    sim_objects.push_back(v);
}

void Model::addWarehouse(const std::shared_ptr<Warehouse>& w) {
    warehouses.push_back(w);
    sim_objects.push_back(w);
}

const std::vector<std::shared_ptr<Warehouse>>& Model::getWarehouses() const {
    return warehouses;
}
const std::vector<std::shared_ptr<Vehicle>>& Model::getVehicles() const {
    return vehicles;
}

void Model::go() {
    for(auto const& obj : sim_objects) {
        obj->update();
    }
    time++;
}

void Model::status() const {
    std::cout << std::endl;
    for(const std::shared_ptr<Sim_obj>& sim : Model::sim_objects) {
        sim->broadcast_current_state();
    }
    std::cout << std::endl;
}

void Model::incrementTime(double hours) {
    time += hours;
}

double Model::getTime() const {
    return time;
}


void Model::insert_objects() {
    for(auto const& obj : vehicles) {
        double min_x = view->getX();
        double max_x = min_x + view->getScale()*(view->getSize() - 1);
        double min_y = view->getY();
        double max_y = min_y + view->getScale()*(view->getSize() - 1);
        if(static_cast<int>(obj->get_location()->x) < min_x || static_cast<int>(obj->get_location()->x) > max_x ||
                static_cast<int>(obj->get_location()->y) < min_y || static_cast<int>(obj->get_location()->y) > max_y) {
            continue;
                }

        std::string tmp = obj->getName();
        view->insert_obj(static_cast<int>(obj->get_location()->x), static_cast<int>(obj->get_location()->y), tmp.substr(0, 2));
    }
    for(auto const& obj : warehouses) {
        double min_x = view->getX();
        double max_x = min_x + view->getScale()*(view->getSize() - 1);
        double min_y = view->getY();
        double max_y = min_y + view->getScale()*(view->getSize() - 1);
        if(obj->get_location()->x < min_x || obj->get_location()->x > max_x ||
           obj->get_location()->y < min_y || obj->get_location()->y > max_y) {
            continue;
           }

        std::string tmp = obj->getName();
        view->insert_obj(static_cast<int>(obj->get_location()->x), static_cast<int>(obj->get_location()->y), tmp.substr(0, 2));
    }
}

const shared_ptr<View> & Model::getView() const {
    return view;
}

std::shared_ptr<Sim_obj> Model::findSimObjByName(const std::string &name) {
    for (const auto& obj : sim_objects) {
        if (obj->getName() == name) {
            return obj;
        }
    }
    return nullptr;
}

double Model::parseTime(const std::string& timeStr) {
    int hours = 0, minutes = 0;
    char colon;
    std::istringstream iss(timeStr);
    iss >> hours >> colon >> minutes;

    if (!iss || colon != ':' || hours < 0 || minutes < 0 || minutes >= 60) {
        std::cerr << "ERROR: Invalid time format: " << timeStr << "\n";
        return 0.0;
    }

    return hours + minutes / 60.0;
}

std::shared_ptr<Vehicle> Model::findVehicleByName(const string &name) {

    for (const auto& obj : vehicles) {
        if (obj->getName() == name) {
            return obj;
        }
    }
    return nullptr;

}

std::shared_ptr<Warehouse> Model::findWarehouseByName(const string &name) {

    for (const auto& obj : warehouses) {
        if (obj->getName() == name) {
            return obj;
        }
    }
    return nullptr;
}
