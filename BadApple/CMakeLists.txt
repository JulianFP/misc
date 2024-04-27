# Set minimum Cmake version
cmake_minimum_required(VERSION 3.11)
# Start project and set its name
project(BadApple LANGUAGES CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(BadApple BadApple.cc)
target_link_libraries(BadApple ${OpenCV_LIBS})
target_link_libraries(BadApple sfml-audio)
