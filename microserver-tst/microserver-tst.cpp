//
// microserver-tst.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "gtest/gtest.h"
#include "gmock/gmock.h"

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
//  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
