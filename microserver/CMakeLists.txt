cmake_minimum_required(VERSION 3.3)
project(microserver)

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

add_library(microserver STATIC ${SOURCE_FILES} ${HEADER_FILES})

