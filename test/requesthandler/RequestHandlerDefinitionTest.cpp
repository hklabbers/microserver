//
// RequestHandlerDefinitionTest.cpp
//
//  Copyright (c) December 2015 Hans Klabbers
//
#include "gtest/gtest.h"
#include "../../microserver/requesthandler/RequestHandlerDefinition.h"

namespace RequestHandlerDefinitionTest {

TEST(RequestHandlerDefinitionTests, TestObjectConstruction) {
  RequestHandlerDefinition rhd{"ThisIsATestUri", "ThisIsATestLibraryName",
                               "ThisIsATestPath", "ThisIsATestSuffix",
                               "ThisIsATestClassname"};
  EXPECT_EQ("ThisIsATestUri", rhd.getUri());
  EXPECT_EQ("ThisIsATestLibraryName", rhd.getLibraryName());
  EXPECT_EQ("ThisIsATestPath", rhd.getPath());
  EXPECT_EQ("ThisIsATestClassname", rhd.getClassName());
}

} // end namespace loginRequestHandlerTest
