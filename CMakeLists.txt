cmake_minimum_required(VERSION 3.16)
project(simDepot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# All simulation logic lives in a static library so that both the
# executable and the test runner can link against the same code.
add_library(sim_core STATIC
        Truck.cpp
        State_trooper.cpp
        Chopper.cpp
        Sim_obj.cpp
        Geometry.cpp
        Vehicle.cpp
        Controller.cpp
        Model.cpp
        View.cpp
        Warehouse.cpp)

target_include_directories(sim_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(sim_core PRIVATE /W4)
else ()
    target_compile_options(sim_core PRIVATE -Wall -Wextra)
endif ()

add_executable(simDepot main.cpp)
target_link_libraries(simDepot PRIVATE sim_core)

# Tests
enable_testing()
add_subdirectory(tests)
