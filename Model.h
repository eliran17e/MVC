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


/**
 * @class Model
 * @brief Singleton class representing the simulation world state.
 *
 * The Model maintains all simulation objects (vehicles, warehouses, etc.),
 * provides global time, and exposes functions to modify/query the simulation.
 * Implements the Singleton pattern: use Model::getInstance().
 */



class Model {
private:
    std::vector<std::shared_ptr<Sim_obj>> sim_objects;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<std::shared_ptr<Warehouse>> warehouses;
    std::shared_ptr<View> view;                                 // View object for displaying the simulation
    double time;


    /**
    * @brief Private constructor for Singleton pattern.
    *        Initializes simulation with a default Frankfurt warehouse.
    */
    Model() {
        view = std::make_shared<View>();
        time = 0;
        auto frankfurt = std::make_shared<Warehouse>("Frankfurt", std::make_shared<Point>(40.0, 10.0), 100000);

        addWarehouse(frankfurt);
    }

public:
    /**
     * @brief Get the singleton instance of the Model.
     * @return Reference to the Model instance.
     */
    static Model& getInstance() {
        static Model instance;
        return instance;
    }

    // Disallow copying
    Model(const Model&) = delete;
    void operator=(const Model&) = delete;

    // Disallow move
    Model(Model&&) = delete;
    Model& operator=(Model&&) = delete;


    /**
     * @brief Find a simulation object by name.
     * @param name Name of the simulation object.
     * @return Shared pointer to the object, or nullptr if not found.
     */
    std::shared_ptr<Sim_obj> findSimObjByName(const string &name);


    /**
    * @brief Parse a time string (e.g. "10:30") into a double (hours).
    * @param timeStr Time string.
    * @return Time as double (e.g. 10.5 for "10:30").
    */
    double parseTime(const std::string &timeStr);


    /**
     * @brief Find a vehicle by name.
     * @param name Name of the vehicle.
     * @return Shared pointer to the vehicle, or nullptr if not found.
     */
    std::shared_ptr<Vehicle> findVehicleByName(const string & name);




    /**
     * @brief Find a warehouse by name.
     * @param name Name of the warehouse.
     * @return Shared pointer to the warehouse, or nullptr if not found.
     */
    std::shared_ptr<Warehouse> findWarehouseByName(const string & string);



    /**
     * @brief Add a simulation object.
     * @param obj Shared pointer to the object.
     */
    void addSimObj(const std::shared_ptr<Sim_obj>& obj);

    /**
    * @brief Add a vehicle to the simulation.
    * @param v Shared pointer to the vehicle.
    */
    void addVehicle(const std::shared_ptr<Vehicle>& v);


    /**
     * @brief Add a warehouse to the simulation.
     * @param w Shared pointer to the warehouse.
     */
    void addWarehouse(const std::shared_ptr<Warehouse>& w);

    /**
    * @brief Get the vector of warehouses.
    * @return Const reference to the warehouse vector.
    */
    const std::vector<std::shared_ptr<Warehouse>>& getWarehouses() const;

    /**
     * @brief Get the vector of vehicles.
     * @return Const reference to the vehicle vector.
     */
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;


    /**
     * @brief Increment simulation time.
     * @param hours Number of hours to increment (default 1.0).
     */
    void incrementTime(double hours = 1.0);


    /**
    * @brief Get the current simulation time (hours).
    * @return Current time.
    */
    double getTime() const;

    /**
     * @brief Print the status of all simulation objects.
     */
    void status() const;

    /**
     * @brief Advance the simulation by one time unit.
     *        Calls update on all simulation objects.
     */
    void go();

    /**
     * @brief Insert existing simulation objects into the View for display.
     */
    void insert_objects();


    /**
     * @brief Get the shared View object.
     * @return Const reference to shared pointer to View.
     */
    const shared_ptr<View> &getView() const;
};
#endif //MODEL_H
