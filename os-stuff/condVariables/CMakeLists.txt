# Set minimum Cmake version
cmake_minimum_required(VERSION 3.11)
# Start project and set its name
project(condVariables LANGUAGES C)
# set build flags
SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread -lrt")
# Add executable
add_executable(condVariables main.c)
