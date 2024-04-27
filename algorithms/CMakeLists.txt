# Set minimum Cmake version
cmake_minimum_required(VERSION 3.11)
# Start project and set its name
project(algorithms LANGUAGES CXX)

#set c++ version to 20
set(CMAKE_CXX_STANDARD 20)

# libraries 
add_library(shortest_path shortest_path.cc)
add_library(graph graph.cc)
# Add executable
add_executable(algorithms main.cc)

target_link_libraries(algorithms PUBLIC shortest_path
    graph
    )
