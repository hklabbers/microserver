file (GLOB_RECURSE MICROSERVER-APP-SRC "*.cpp")
file (GLOB_RECURSE MICROSERVER-APP-HDR "*.h")

set (MICROSERVER-APP-INCLUDE-DIRS "")
foreach (_headerFile ${MICROSERVER-APP-HDR})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND MICROSERVER-APP-INCLUDE-DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES MICROSERVER-APP-INCLUDE-DIRS)

list (APPEND resources/microserver-app.properties)

# Create executable hmd-be-app
add_executable (microserver-app ${MICROSERVER-APP-SRC})

target_link_libraries (microserver-app LINK_PUBLIC microserver-lib PocoCrypto PocoFoundation PocoNet PocoUtil PocoJSON)
# target_link_libraries (microserver-app LINK_PUBLIC PocoCrypto PocoFoundation PocoNet PocoUtil PocoJSON)
