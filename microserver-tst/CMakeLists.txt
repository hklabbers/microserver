file (GLOB_RECURSE MICROSERVER-TST-SRC "*.cpp")
file (GLOB_RECURSE MICROSERVER-TST-HDR "*.h")

set (MICROSERVER-TST-INCLUDE-DIRS "")
foreach (_headerFile ${MICROSERVER-TST-HDR})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND MICROSERVER-TST-INCLUDE-DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES MICROSERVER-TST-INCLUDE-DIRS)

# Googlemock with googletest
set (GOOGLEMOCK_SOURCE_DIR "${PROJECT_SOURCE_DIR}/external-libraries/googletest/googlemock")
set (GOOGLEMOCK_BINARY_DIR "${PROJECT_BINARY_DIR}/external-libraries/googletest/googlemock")

set (GMOCK_LIBRARY "${GOOGLEMOCK_BINARY_DIR}/libgmock.a")
set (GTEST_LIBRARY "${GOOGLEMOCK_BINARY_DIR}/gtest/libgtest.a")

set (GMOCK_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external-libraries/googletest/googlemock/include")
set (GTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external-libraries/googletest/googletest/include")

include_directories ("${GTEST_INCLUDE_DIR}")
include_directories ("${GMOCK_INCLUDE_DIR}")

# Create executable hmd-be-tst
add_executable (microserver-tst ${MICROSERVER-TST-SRC})

target_link_libraries (microserver-tst LINK_PUBLIC microserver-lib ${GTEST_LIBRARY} ${GMOCK_LIBRARY} pthread PocoCrypto PocoFoundation PocoNet PocoJSON)
