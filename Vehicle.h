#ifndef VEHICLE_H
#define VEHICLE_H
#include <string>
#include <memory>
#include "Sim_obj.h"
#include "math.h"


/**
 * @class Vehicle
 * @brief Abstract base class representing a movable simulation object (e.g. truck, chopper, state trooper).
 *
 * A Vehicle can move by position, by course, or follow a destination/warehouse mode.
 * Contains shared movement logic and fields for all derived vehicle types.
 */
class Vehicle :   public Sim_obj {
protected:
    std::string state;
    shared_ptr<Point> _location;

    double angle;                       // in degrees (0-360)
    double speed;                       // in km/h
    shared_ptr<Point> destination;      // optional, only when moving to a location
    int mode = 0; // 0 for position, 1 for in course , 2 destination/warehouse mode for trucks and state troopers
    bool en_route = false;
    bool pending_position = false;
    bool pending_course = false;
    double pending_pos_x = 0, pending_pos_y = 0, pending_pos_speed = 0;
    double pending_course_angle = 0, pending_course_speed = 0;

    /**
     * @brief Configure position mode toward the pending (x, y) target.
     * @param worldSpeed Speed in world units per tick (already scaled).
     *
     * Sets the destination, heading (compass convention, 0 deg = North),
     * speed and mode. Callers set their own state string afterwards.
     */
    void beginPositionMode(double worldSpeed);

    /**
     * @brief Configure course mode using the pending angle.
     * @param worldSpeed Speed in world units per tick (already scaled).
     */
    void beginCourseMode(double worldSpeed);

    /**
     * @brief Advance the location one step along the current heading.
     * @param stepDistance Distance to move this tick (world units).
     *
     * Uses the compass convention (0 deg = North/+y, 90 deg = East/+x).
     */
    void stepAlongHeading(double stepDistance);

public:
    Vehicle();


    /**
     * @brief Constructor
     * @param name Vehicle name
     * @param location Initial location (shared pointer)
     * @param state Initial state (default: "Stopped")
     */
    Vehicle(std::string name, shared_ptr<Point> location, std::string state = "Stopped")
        : Sim_obj(std::move(name)), state(std::move(state)),
    _location(std::move(location)), angle(0.0), speed(0.0), destination(nullptr) {}


    ~Vehicle() override=default;

    /**
     * @brief Get current location
     * @return Reference to location pointer
     */
    const shared_ptr<Point> &get_location() const override;

    /**
     * @brief Set the current location
     * @param location Pointer to new location
     */
    void set_location(const shared_ptr<Point> &location) ;

    /**
    * @brief Update vehicle position/state (pure virtual, implemented in derived classes)
    */
    void update() override = 0;


    /**
     * @brief Print or broadcast the vehicle's current state (pure virtual)
     */
    void broadcast_current_state() override = 0;


    /**
     * @brief Initiate movement on a course (angle/speed); sets pending course.
     * @param angle Direction in degrees (0 = North)
     * @param speed Speed in km/h
     */
    void course(double angle, double speed);


    /**
     * @brief Initiate movement to a position (x/y); sets pending position.
     * @param x Target X coordinate
     * @param y Target Y coordinate
     * @param speed Speed in km/h
     */
    void position(double x, double y, double speed);

    /**
     * @brief Get the vehicle's state string.
     * @return State string
     */
    const std::string &getState() const {
        return state;
    }


    /**
     * @brief Set the vehicle's state string.
     * @param state1 New state
     */
    void setState(const std::string &state1) {
        state = state1;
    }


    /**
     * @brief Stop the vehicle, set state to stopped, and halt movement.
     */
    virtual void stop();



    /**
     * @brief Compute distance between two points (static utility).
     * @param x1, y1 First point
     * @param x2, y2 Second point
     * @return Euclidean distance
     */
    static double computeDistance(double x1, double y1, double x2, double y2);


    /**
     * @brief Set the vehicle's speed (km/h)
     * @param x New speed
     */
    void setSpeed(double x);

    /**
     * @brief Get current speed
     * @return Speed in km/h
     */
    double getSpeed() const { return speed; }


    /**
     * @brief Get current movement angle (degrees)
     * @return Angle in degrees (0-360)
     */
    double getAngle() const { return angle; }

    /**
     * @brief Set the vehicle's movement angle (degrees)
     * @param angle Angle to set
     */
    void setAngle(double angle) { this->angle = angle; }

};



#endif //VEHICLE_H
