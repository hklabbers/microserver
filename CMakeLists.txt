cmake_minimum_required (VERSION 3.1)
project(microserver)
set (microserver_VERSION_MAJOR 0)
set (microserver_VERSION_MINOR 1)

set (CMAKE_VERBOSE_MAKEFILE ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O0 -g3 -Wall -fmessage-length=0")

# First add external libraries
add_subdirectory(external-libraries)
add_subdirectory(microserver-app)
