#include "Model.h"

#include <sstream>

#include "Chopper.h"

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
        if (!dynamic_cast<Chopper*>(obj.get())) {
            obj->update();
        }
    }
    for(auto const& obj : vehicles) {
        if (dynamic_cast<Chopper*>(obj.get())) {
            obj->update();
        }
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


    for (auto const& obj : vehicles) {
        auto loc = obj->get_location();
        double scale = view->getScale();
        int size = view->getSize();

        int col = static_cast<int>(loc->x);
        int row = static_cast<int>(loc->y);

        if (col < 0 || col/scale >= size || row < 0 || row/scale >= size)
            continue;  // Out of view bounds

        view->insert_obj(row, col, obj->getName().substr(0, 2));
    }

    for (auto const& obj : warehouses) {
        auto loc = obj->get_location();
        double scale = view->getScale();
        int size = view->getSize();
        int col = static_cast<int>(loc->x);
        int row = static_cast<int>(loc->y);

        if (col < 0 || col/scale >= size || row < 0 || row/scale >= size)
            continue;

        view->insert_obj(row, col, obj->getName().substr(0, 2));
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
