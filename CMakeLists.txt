cmake_minimum_required(VERSION 3.0)
project(ms)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g3 -Wall -fmessage-length=0")

# Poco libraries used by this project
set(POCO PocoFoundation PocoNet PocoUtil)

#Boost libraries used by this project
find_package(Boost 1.36.0 COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

# Build gtest and googlemock
add_subdirectory(test)

add_subdirectory(microserver)

add_executable(ms ms.cpp)
target_link_libraries(ms microserver ${POCO} ${Boost_LIBRARIES})

file(COPY resources/ms.properties DESTINATION ${CMAKE_BINARY_DIR})

