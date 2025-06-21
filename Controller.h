//
// Created by User on 20/06/2025.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>


class Controller {
public:
    Controller () = default;
    ~Controller() = default;

    // creates View object, runs the program by accepting user commands, then destroys View object
    void run(int argc, char* argv[]);

    void loadWarehouses(const std::string &filename);

    void loadTruckSchedule(const std::string &filename);

    void commandLoop();



};
#endif //CONTROLLER_H
