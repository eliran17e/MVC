//
// Created by User on 20/06/2025.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
/**
 * @class Controller
 * @brief Responsible for managing the simulation flow and handling user commands.
 *
 * The Controller handles:
 *  - Initialization of the simulation.
 *  - Loading warehouse and truck data from files.
 *  - Running the main command loop (user interaction).
 */

class Controller {
public:


    /**
     * @brief Constructs a Controller object.
     */
    Controller ();
    ~Controller() = default;

    /**
     * @brief Main entry point for running the simulation.
     *
     * @param argc Number of command-line arguments.
     * @param argv Command-line argument array.
     *
     * This method initializes the View, loads initial data, and
     * enters the main user command loop until program exit.
     */
    void run(int argc, char* argv[]);


    /**
         * @brief Loads warehouse data from the specified file.
         * @param filename Path to the file containing warehouse definitions.
         */
    void loadWarehouses(const std::string &filename);


    /**
    * @brief Loads truck schedule data from the specified file.
    * @param filename Path to the file containing a truck's delivery schedule.
    */
    void loadTruckSchedule(const std::string &filename);


    /**
    * @brief Runs the main command loop, accepting and executing user commands.
    *
    * Handles commands such as 'go', 'status', 'show', 'create', and object-specific commands.
    */
    void commandLoop();



};
#endif //CONTROLLER_H
