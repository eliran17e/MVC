//
// Created by User on 20/06/2025.
//

#ifndef VIEW_H
#define VIEW_H



#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

class View {
private:
    std::vector<std::vector<std::string>> map;      // Map representing the view
    int size;                                       // Size of the view
    double scale;                                   // Scale factor for zooming
    double x;                                       // X-coordinate
    double y;                                       // Y-coordinate

    // Helper method to rotate the view matrix
    std::vector<std::vector<std::string>> rotate_matrix();

public:
    View();         // Default constructor

    // Big three
    View(const View &v);                // Copy constructor
    ~View() = default;                  // Destructor
    View &operator=(const View &v);     // Copy assignment operator

    void default_size();                // Set the view size to default
    void new_size(int s);               // Set a new view size
    void new_zoom(double z);            // Set a new zoom scale factor
    void pan(double xx, double yy);     // Pan the view by adjusting the coordinates
    void show();                        // Display the view

    void insert_obj(double i, double j, std::string name);      // Insert an object into the view
    void clear();                       // Clear the view

    // Getters
    int getSize() const;                // Get the view size
    double getScale() const;            // Get the zoom scale factor
    double getX() const;                // Get the X-coordinate
    double getY() const;                // Get the Y-coordinate for panning
};


#endif //VIEW_H
