//
// Created by User on 20/06/2025.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H



class Controller {
public:
    Controller();
    ~Controller();

    // creates View object, runs the program by accepting user commands, then destroys View object
    void run();

private:
    View* view_ptr;
};
#endif //CONTROLLER_H
