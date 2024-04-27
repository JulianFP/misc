# Set minimum Cmake version
cmake_minimum_required(VERSION 3.11)
# Start project and set its name
project(minishell LANGUAGES CXX)
# Add executable
add_executable(minishell minishell.cc)
