//
// Created by User on 20/06/2025.
//

#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#include "Sim_obj.h"


class Warehouse : public Sim_obj{
private:
    int _inventory;
    int _quantity;
    std::shared_ptr<Point> _location;
public:
    Warehouse(const string &n,std::shared_ptr<Point> location,int inventory) ;
    ~Warehouse() override = default;
    const shared_ptr<Point> &get_location() const override;
    void update() override;
    void update_boxes(int quantity);
    void broadcast_current_state() override;



};



#endif //WAREHOUSE_H
