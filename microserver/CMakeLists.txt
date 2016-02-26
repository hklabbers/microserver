cmake_minimum_required(VERSION 3.3)
project(microserver)

set (CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g3 -Wall -fmessage-length=0")


set(HEADER_FILES requesthandler/MicroServerRequestHandlerFactory.h
        requesthandler/AbstractRequestHandler.h
        requesthandler/RequestHandlerDefinition.h
        requesthandler/StatusRequestHandler.h
        requesthandler/Error404RequestHandler.h
        )

set(SOURCE_FILES requesthandler/MicroServerRequestHandlerFactory.cpp
        requesthandler/RequestHandlerDefinition.cpp
        requesthandler/StatusRequestHandler.cpp
        requesthandler/Error404RequestHandler.cpp
        )

add_library(microserver STATIC ${SOURCE_FILES} ${HEADER_FILES} )

