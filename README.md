# simDepot — Vehicle & Logistics Simulation

[![CI](https://github.com/eliran17e/MVC/actions/workflows/ci.yml/badge.svg)](https://github.com/eliran17e/MVC/actions/workflows/ci.yml)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

A command-driven simulation of a logistics-and-law-enforcement scenario, written
in modern C++ around a clean **Model–View–Controller** architecture. Trucks run
scheduled deliveries between warehouses, state troopers patrol, and choppers
hunt trucks — all advancing in discrete time steps and rendered on an ASCII map
you can pan and zoom.

```text
Display size: 16, scale: 7.00, origin: (0.00, 0.00)
105.  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
84 .  .  Ha .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  Be .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
63 .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  He .  .  .  .  .  .  .  .  .  .  .
42 .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  Co .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  Mu .  .  .  .  .  .  .
21 .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
   .  .  .  .  .  Fr .  .  .  .  .  .  .  .  .  .
0  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
```
*(Ha=Hamburg, Be=Berlin, He=Chopper, Co=Cologne, Mu=Munich, Fr=Frankfurt)*

## Features

- **Three vehicle types**, each with distinct behavior:
  - `Truck` — follows a pre-loaded delivery schedule between warehouses,
    unloading crates on arrival; can also be driven manually.
  - `State_trooper` — moves to a warehouse on command, then auto-patrols to the
    nearest unvisited warehouse.
  - `Chopper` — flies by position or course and can attack a truck in range when
    no trooper is nearby; its attack range grows on success and shrinks on
    failure.
- **Warehouses** with buffered inventory that settles each tick.
- **Pan / zoom / resize** ASCII map view with labelled axes.
- **File-driven setup** for warehouses and truck schedules, plus an interactive
  command loop.

## Architecture

The project is a textbook MVC split, with a small geometry layer underneath.

```
            +--------------+        commands         +-------------+
   stdin -->|  Controller  |------------------------>|    Model    |
            +--------------+                          |  (singleton) |
                   |                                   +------+------+
                   | loads data                               | owns
                   v                                          v
            depot.dat / *.txt                    Sim_obj  ── Warehouse
                                                    └── Vehicle ── Truck
                                                                ├ Chopper
            +--------------+      renders               └ State_trooper
            |     View     |<------------------------------- Model
            +--------------+   (ASCII map, pan/zoom)
```

- **`Model`** — owns all simulation objects, the global clock, and the `View`;
  advances the world each `go`. Updates run in two passes so choppers resolve
  attacks against everyone's final positions (`Sim_obj::defersUpdate`).
- **`View`** — renders a grid map; maps world `(x, y)` to grid cells using the
  current origin and scale.
- **`Controller`** — parses CLI arguments and data files, then runs the
  interactive command loop.
- **`Geometry`** — `Point`, polar/Cartesian vectors, and angle conversions.

All simulation code is compiled into a `sim_core` static library that both the
`simDepot` executable and the test runner link against.

## Build

Requires a C++17 compiler and CMake ≥ 3.16.

```sh
cmake -S . -B build
cmake --build build
```

The executable is produced at `build/simDepot`. A plain `Makefile` is also
provided for a CMake-free build (`make`).

## Run

```sh
./build/simDepot -w <warehouse_file> -t <route_file> [<route_file> ...]
```

A ready-to-run scenario lives in [`data/`](data):

```sh
./build/simDepot -w data/depot.dat -t data/route_north.txt data/route_south.txt
# or, with the Makefile:
make run
```

A warehouse named **Frankfurt** is always created as the home depot, so route
files can depart from it without listing it in the warehouse file.

### Data formats

**Warehouse file** — one warehouse per line:

```
Name, (x, y), crates
Munich, (60.0, 30.0), 500
```

**Route file** — start line, then one delivery leg per line. The file name
(without extension) becomes the truck's name:

```
StartWarehouse, HH:MM                 # start location and departure time
Warehouse, HH:MM, crates, HH:MM       # destination, arrival, crates, departure
Munich, 10:00, 50, 11:00
```

## Commands

| Command | Description |
| --- | --- |
| `go` | Advance the simulation by one time unit. |
| `status` | Print the status of every object. |
| `show` | Render the current map. |
| `create <name> State_trooper <warehouse>` | Create a trooper at a warehouse. |
| `create <name> Chopper (x, y)` | Create a chopper at coordinates. |
| `<vehicle> position <x> <y> [speed]` | Move toward a position. |
| `<vehicle> course <angle> [speed]` | Move on a heading. |
| `<trooper> destination <warehouse>` | Send a trooper to a warehouse. |
| `<chopper> attack <truck>` | Attempt to attack a truck. |
| `<vehicle> stop` | Stop the vehicle. |
| `zoom <factor>` / `size <n>` / `pan <x> <y>` / `default` | Adjust the view. |
| `exit` | Quit. |

### Coordinate & angle conventions

A 2-D `(x, y)` plane. Headings use a **compass** convention:

- `0°` = North (+y), `90°` = East (+x), `180°` = South, `270°` = West.
- Angles increase clockwise.

## Tests

Unit tests use [doctest](https://github.com/doctest/doctest) (vendored under
`tests/third_party`) and run via CTest:

```sh
ctest --test-dir build --output-on-failure
```

They cover geometry conversions, warehouse inventory rules, time parsing,
name lookups, distance, and the vehicle movement convention. CI runs the build
and the full suite on Linux and macOS for every push and pull request.

## Project layout

```
.
├── main.cpp                # entry point
├── Controller.*            # CLI parsing, file loading, command loop
├── Model.*                 # world state, clock, update loop (singleton)
├── View.*                  # ASCII map rendering
├── Sim_obj.* / Vehicle.*   # object hierarchy base classes
├── Truck.* / Chopper.* / State_trooper.*
├── Warehouse.*             # inventory locations
├── Geometry.*              # points, vectors, angle math
├── data/                   # ready-to-run sample scenario
└── tests/                  # doctest-based unit tests
```

## License

Released under the [MIT License](LICENSE).
