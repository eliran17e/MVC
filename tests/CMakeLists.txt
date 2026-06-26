add_executable(unit_tests
        test_main.cpp
        test_geometry.cpp
        test_warehouse.cpp
        test_model.cpp
        test_vehicle.cpp)

target_link_libraries(unit_tests PRIVATE sim_core)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

add_test(NAME unit_tests COMMAND unit_tests)
