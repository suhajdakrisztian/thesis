#include "../../src/p_iota.h"

#include <stdlib.h>
#include <gtest/gtest.h>

TEST(IOTA, GeneratedCorrectly) {
  
  std::vector<int> actual(10, 0);
  std::vector<int> expected{2,3,4,5,6,7,8,9,10,11};

  parallel::iota(actual.begin(), actual.end(), 2);
  
  EXPECT_EQ(actual, expected);
}

TEST(IOTA, GeneratedCorrectlyNegative) {
  
  std::vector<int> actual(10, 0);
  std::vector<int> expected{-10, -9, -8,-7,-6,-5,-4,-3,-2,-1};

  parallel::iota(actual.begin(), actual.end(), -10);
  
  EXPECT_EQ(actual, expected);
}

TEST(IOTA, IntMin) {

  constexpr auto INT_MIN = -2147483648;
  std::vector<int> actual(5, 0);
  parallel::iota(actual.begin(), actual.end(), INT_MIN);
  
  std::vector<int> expected{INT_MIN, INT_MIN+1, INT_MIN+2,INT_MIN+3,INT_MIN+4};

  EXPECT_EQ(actual, expected);
}

TEST(IOTA, RandomStartPoint) {
  
  std::vector<int> actual(11, 0);
  auto start_point = rand() % 100 + 1;

  parallel::iota(actual.begin(), actual.end(), start_point);
  
  std::vector<int> expected;
  for(auto i = start_point; i < start_point + actual.size(); i++) {
    expected.push_back(i);
  }

  EXPECT_EQ(actual, expected);
}

TEST(IOTA, RandomStartPointLargeLoad) {
  
  std::vector<int> actual(100001, 0);
  auto start_point = rand() % 100 + 1;

  parallel::iota(actual.begin(), actual.end(), start_point);
  
  std::vector<int> expected;
  for(auto i = start_point; i < start_point + actual.size(); i++) {
    expected.push_back(i);
  }

  EXPECT_EQ(actual, expected);
}

TEST(IOTA, stdIota) {
  
  std::vector<int> actual(11111111, 0);
  auto start_point = rand() % 100 + 1;

  std:iota(actual.begin(), actual.end(), start_point);
  
  std::vector<int> expected;
  for(auto i = start_point; i < start_point + actual.size(); i++) {
    expected.push_back(i);
  }

  EXPECT_EQ(actual, expected);
}