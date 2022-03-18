cmake_minimum_required (VERSION 3.21)
project ("Engine")

# enable output of compile commands / this doesn't work =(
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include sub-projects.
add_subdirectory ("Engine")
add_subdirectory ("Demo")
