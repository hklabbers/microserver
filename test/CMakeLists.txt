project(test)

add_subdirectory(lib/gtest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

set(TEST_SOURCE_FILES ms-tst.cpp mock/MockHTTPRequest.h
        mock/MockHTTPResponse.h
        requesthandler/RequestHandlerDefinitionTest.cpp
        requesthandler/MicroServerRequestHandlerFactoryTest.cpp)

add_executable(runTests
        ${TEST_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES}
        )

target_link_libraries(runTests gmock gtest_main microserver ${POCO})
#target_link_libraries(runTests)