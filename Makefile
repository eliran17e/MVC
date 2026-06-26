# Convenience Makefile for a quick build without CMake.
# For tests and a full configured build, use CMake (see README).

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET    = simDepot

# Every .cpp except the test sources builds the simulator.
SRCS = main.cpp Controller.cpp Model.cpp View.cpp Warehouse.cpp \
       Vehicle.cpp Truck.cpp Chopper.cpp State_trooper.cpp \
       Sim_obj.cpp Geometry.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Quick run against the bundled sample scenario.
run: $(TARGET)
	./$(TARGET) -w data/depot.dat -t data/route_north.txt data/route_south.txt

clean:
	rm -f $(OBJS) $(TARGET)
