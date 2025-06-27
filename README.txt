# Vehicle Simulation Project

## Overview

This project simulates a logistics and law-enforcement scenario involving trucks,
choppers, and state troopers, moving between warehouses on a coordinate map.
The system is designed using the MVC (Model-View-Controller) pattern in C++.
Users interact with the system by entering commands, and the simulation advances in discrete time steps.

## Structure

- **Model:**
  Holds all simulation objects (`Vehicle`, `Warehouse`) and manages the simulation state, time, and object lookup.

- **View:**
  Displays an ASCII map/grid representing the positions of all vehicles and warehouses.
  Allows panning, zooming, and resizing.

- **Controller:**
  Handles user input, loads data from files, parses commands, and triggers simulation updates.

- **Vehicles:**
  - `Truck`: Follows scheduled delivery routes between warehouses and unloads cargo.
  - `Chopper`: Can move by position or course, and may attack trucks within range if no police are nearby.
  - `State_trooper`: Patrols between warehouses and can move by position, course, or destination.

- **Warehouse:**
  Represents a location with a name, coordinates, and inventory.

## Coordinate System & Angles

The coordinate system is 2D (x, y).
**Angle conventions used in this simulation:**
- **0 degrees:** North (positive y direction)
- **90 degrees:** East (positive x direction)
- **180 degrees:** South (negative y direction)
- **270 degrees:** West (negative x direction)
- Angles increase clockwise.

All vehicle movements are based on this system. For example, a course of 0 degrees moves a vehicle north (up the map), and 90 degrees moves it east (right).

## User Commands

- `go`: Advance the simulation by one time unit.
- `status`: Print the current status of all objects.
- `create <name> State_trooper <warehouse>`: Create a state trooper at a warehouse.
- `create <name> Chopper (x, y)`: Create a chopper at coordinates.
- `<vehicle> position <x> <y> <speed>`: Schedule vehicle to move to position.
- `<vehicle> course <angle> <speed>`: Schedule vehicle to move at angle/speed.
- `<vehicle> destination <warehouse>`: Send trooper to a warehouse.
- `<chopper> attack <truck>`: Attempt to attack a truck (must be in range and no police nearby).
- `<vehicle> stop`: Stop vehicle.
- `show`: Show the current map view.
- `zoom <factor>`, `size <size>`, `pan <x> <y>`, `default`: Adjust view.

## Logic Highlights

- **Pending commands:** All vehicle movement and actions (course/position/attack) are scheduled and executed at the next simulation step.
- **Attack logic:** Choppers may attack a truck if within range and no state trooper is within 10 km of the truck.
- **Truck delivery:** Trucks deliver cargo along preloaded routes, updating warehouse inventories as they arrive.
- **State trooper patrol:** Troopers can patrol between warehouses or move manually.

## Compilation and Running

```sh
make
$ ./simDepot –w depot.dat –t <truckfile1> [<truckfile2> <truckfile3> ...]