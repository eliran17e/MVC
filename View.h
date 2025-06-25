//
// Created by User on 20/06/2025.
//

#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>



/**
 * @class View
 * @brief Responsible for displaying the simulation map, panning, zooming, and showing objects' locations.
 *
 * The View class manages a grid-based ASCII map of the simulation,
 * supports zoom, pan, and object insertion, and provides
 * methods to display the current state.
 */
class View {
private:
    std::vector<std::vector<std::string>> map;      // Map representing the view
    int size;                                       // Size of the view
    double scale;                                   // Scale factor for zooming
    double x;                                       // X-coordinate
    double y;                                       // Y-coordinate

    /**
     * @brief Helper function to rotate the map (for display purposes)
     * @return Rotated matrix
     */
    std::vector<std::vector<std::string>> rotate_matrix();

public:

    /**
     * @brief Default constructor (sets default map size/scale/origin)
     */
    View();



    /**
     * @brief Copy constructor
     */
    View(const View &v);

    /**
     * @brief Destructor (default)
     */
    ~View() = default;

    /**
    * @brief Assignment operator (copy)
    */
    View &operator=(const View &v);


    /**
     * @brief Reset the view size and scale to default values
     */
    void default_size();

    /**
    * @brief Change the view size (number of grid cells)
    * @param s New size (width and height)
    */
    void new_size(int s);


    /**
    * @brief Change the zoom scale (km per cell)
    * @param z New zoom factor
    */
    void new_zoom(double z);


    /**
     * @brief Move the view window by changing the lower-left origin coordinates
     * @param xx New X origin
     * @param yy New Y origin
     */
    void pan(double xx, double yy);


    /**
     * @brief Print the view (current map state) to the console
     */
    void show();


    /**
     * @brief Insert an object (by name) at location (i, j) into the view
     * @param i X coordinate
     * @param j Y coordinate
     * @param name Short name/symbol to display on the map
     */
    void insert_obj(double i, double j, std::string name);

    /**
    * @brief Clear the map of all objects (reset grid)
    */
    void clear();

    // Getters
    int getSize() const;                // Get the view size
    double getScale() const;            // Get the zoom scale factor
    double getX() const;                // Get the X-coordinate
    double getY() const;                // Get the Y-coordinate for panning
};


#endif //VIEW_H
