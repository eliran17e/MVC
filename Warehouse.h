//
// Created by User on 20/06/2025.
//

#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#include "Sim_obj.h"

/**
 * @class Warehouse
 * @brief Represents a warehouse in the simulation. Warehouses store inventory and can be restocked or depleted by vehicles.
 *
 * Inherits from Sim_obj. Maintains its location, inventory level, and handles updates from incoming or outgoing deliveries.
 */
class Warehouse : public Sim_obj{
private:
    int _inventory;
    int _quantity;
    std::shared_ptr<Point> _location;
public:

    /**
    * @brief Constructor for Warehouse
    * @param n Name of the warehouse
    * @param location Shared pointer to the warehouse's location
    * @param inventory Initial inventory count
    */
    Warehouse(const string &n,std::shared_ptr<Point> location,int inventory) ;
    ~Warehouse() override = default;

    /**
    * @brief Returns the location of the warehouse.
    * @return Shared pointer to Point representing warehouse location
    */
    const shared_ptr<Point> &get_location() const override;

    /**
     * @brief Update the inventory of the warehouse based on _quantity buffer
     *
     * Typically called every time unit, applies any pending inventory changes.
     */
    void update() override;


    /**
     * @brief Schedule a change in inventory (to be processed in next update)
     * @param quantity Number of boxes to add (positive) or remove (negative)
     * @return true if operation was scheduled
     */
    bool update_boxes(int quantity);

    /**
     * @brief Print the current status of the warehouse (for status command)
     */
    void broadcast_current_state() override;

    /**
     * @brief Get the current inventory level
     * @return Integer count of boxes in inventory
     */
    int getInventory() const;



};



#endif //WAREHOUSE_H
