file (GLOB_RECURSE MICROSERVER-LIB-SRC "*.cpp")
file (GLOB_RECURSE MICROSERVER-LIB-HDR "*.h")

set (MICROSERVER-LIB-INCLUDE-DIRS "")
foreach (_headerFile ${MICROSERVER-LIB-HDR})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND MICROSERVER-LIB-INCLUDE-DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES MICROSERVER-LIB-INCLUDE-DIRS)

find_package(Poco REQUIRED)
include_directories (${Poco_INCLUDE_DIRS})
link_directories (${Poco_LIBRARY_DIR})

add_library (microserver-lib STATIC ${MICROSERVER-LIB-SRC})
target_include_directories(microserver-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
