#include "../../src/copy_backward.h"

#include <gtest/gtest.h>

TEST(COPY_BACKWARD, GeneratedCorrectly) {
  
  std::vector<int> source{2,3,4,5,6,7,8,9,10,11};
  std::vector<int> result{0,0,0,0,0,0,0,0,0,0};

  pstl::copy_backward(source.begin(), source.end(), result.end());

  EXPECT_EQ(result, source);
}

TEST(COPY_BACKWARD, OnlyCopiesRelevant) {
  
  std::vector<int> source{8,9,10,11};
  std::vector<int> result{0,0,0,0,0,0,0,0,0,0};

  pstl::copy_backward(source.begin(), source.end(), result.end());

  std::vector<int> expected{0,0,0,0,0,0,8,9,10,11};

  EXPECT_EQ(result, expected);
}

TEST(COPY_BACKWARD, RandomStartPoint) {
  
  std::vector<int> actual(15);

  std::vector<int> expected;
  auto start_point = rand() % 100 + 1;

  for(int i = start_point; i < start_point + actual.size(); i++) {
    expected.push_back(i);
  }

  pstl::copy_backward(expected.begin(), expected.end(), actual.end());
  
  EXPECT_EQ(actual, expected);
}

TEST(COPY_BACKWARD, RandomStartPointLargeNumber) {
  
  std::vector<int> actual(100001);

  std::vector<int> expected;
  auto start_point = rand() % 100 + 1;

  for(int i = start_point; i < start_point + actual.size(); i++) {
    expected.push_back(i);
  }

  pstl::copy_backward(expected.begin(), expected.end(), actual.end());
  
  EXPECT_EQ(actual, expected);
}

TEST(COPY_BACKWARD, ShouldFail) {

  std::vector<int> v {1, 2, 3, 4, 5, 6};
  std::vector<int> res(3,0);

  pstl::copy_backward(v.begin(), v.end(), res.end());

  std::vector<int> expected{4,5,1};

  EXPECT_EQ(expected, res);
}

