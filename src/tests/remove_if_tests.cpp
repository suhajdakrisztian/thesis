#include "../../src/p_copy_if.h"

#include "gtest/gtest.h"
#include <vector>
#include <execution>

TEST(COPY_IF, ReturnsCorrectNumber) {
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

  std::vector<int> result;

  const auto res = parallel::copy_if(
      std::execution::par, v.begin(), v.end(),result.end() ,[](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(result.size(), 6);
}

TEST(COPY_IF, ReturnsEndWhenNoElement) {
  std::vector<int> v(100'000, 2);
  std::vector<int> result;

  const auto res = parallel::copy_if(
      std::execution::par, v.begin(), v.end(),result.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(result.size(), 0);
}
