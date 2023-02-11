#include <gtest/gtest.h>

#include "p_find_if.h"

TEST(FIND_IF, HasCorrectResult) {
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

  const auto res = pstl::find_if(
      std::execution::par, v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  ASSERT_EQ(555, *res);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}