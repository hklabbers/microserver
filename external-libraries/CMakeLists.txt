cmake_minimum_required (VERSION 3.1)
project(external-libraies)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O0 -g3 -Wall -fmessage-length=0")

add_subdirectory(googletest/googlemock)
