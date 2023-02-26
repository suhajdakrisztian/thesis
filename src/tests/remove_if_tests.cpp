#include "../../src/p_copy_if.h"


#include "gtest/gtest.h"
#include <vector>
#include <execution>
#include <iostream>
#include <vector>


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

  std::vector<int> res;

  parallel::PSTL<std::vector<int>::iterator> p;
  auto qqq = p.copy_if(
      std::execution::par, v.begin(), v.end(),std::back_inserter(res), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter >= 5; });

  EXPECT_EQ(res.size(), 5ul);
}

TEST(COPY_IF, Complement) {
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

  std::vector<int> res;

  parallel::PSTL<std::vector<int>::iterator> p;
  auto qqq = p.copy_if(
      std::execution::par, v.begin(), v.end(),std::back_inserter(res), [](int i) { return i < 50; },
      [](int i) mutable { return i == 2; });

  EXPECT_EQ(res.size(), 100000-10);
}

