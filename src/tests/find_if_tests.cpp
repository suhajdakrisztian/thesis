#include "../../src/p_find_if.h"

#include <gtest/gtest.h>

TEST(FIND_IF, ReturnsCorrectNumber) {

  std::vector<int> v(100'000, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[77777] = 7657;
  v[88888] = 312231;

  auto res = parallel::find_if(
      std::execution::par, v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(555, *res);
}

TEST(FIND_IF, ReturnsEndWhenNoElement) {

  std::vector<int> v(100'000, 2);

  auto res = parallel::find_if(
      std::execution::par, v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(v.end(), res);
}
