cmake_minimum_required(VERSION 3.10)
project(cpp-geometry-utils)
set(CMAKE_CXX_STANDARD 17)
# Include directories
include_directories(include)
# Source files
add_executable(geometry-utils 
    src/main.cpp
    src/Point.cpp 
    src/Line.cpp 
    src/Plane.cpp 
    src/Polygon.cpp 
    src/utils/utils.cpp)
