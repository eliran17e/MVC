//
// Created by User on 20/06/2025.
//

#ifndef MODEL_H
#define MODEL_H



#include <vector>
#include <memory>
#include <string>

#include "Sim_obj.h"
#include "Warehouse.h"
#include "Vehicle.h"
#include "View.h"

class Model {
private:
    std::vector<std::shared_ptr<Sim_obj>> sim_objects;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<std::shared_ptr<Warehouse>> warehouses;
    std::shared_ptr<View> view;                                 // View object for displaying the simulation

    double time;

    Model() {
        view = std::make_shared<View>();
        time = 0;
    }

public:
    // Singleton accessor
    static Model& getInstance() {
        static Model instance;
        return instance;
    }

    // Disallow copying
    Model(const Model&) = delete;
    void operator=(const Model&) = delete;

    std::shared_ptr<Sim_obj> findSimObjByName(const string &name);

    double parseTime(const std::string &timeStr);

std::shared_ptr<Vehicle> findVehicleByName(const string & name);

std::shared_ptr<Warehouse> findWarehouseByName(const string & string);

    Model(Model&&) = delete;
    Model& operator=(Model&&) = delete;

    // Add methods
    void addSimObj(const std::shared_ptr<Sim_obj>& obj);
    void addVehicle(const std::shared_ptr<Vehicle>& v);
    void addWarehouse(const std::shared_ptr<Warehouse>& w);

    const std::vector<std::shared_ptr<Warehouse>>& getWarehouses() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;


    void incrementTime(double hours = 1.0);
    double getTime() const;

    // Print the status of all simulation objects
    void status() const;

    // Advances the simulation by one time point
    void go();

    // Insert the existing objects into the display
    void insert_objects();

    const shared_ptr<View> &getView() const;
};
#endif //MODEL_H
