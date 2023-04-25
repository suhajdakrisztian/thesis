#include "../../src/p_copy_if.h"

#include "gtest/gtest.h"
#include <execution>
#include <iostream>
#include <vector>

TEST(COPY_IF, BothConditionsMetMultipleResults) {
  std::vector<int> v(100'000, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  std::vector<int> result;

  pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(result),
      [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter >= 5; });

  std::vector<int> expected{555, 7657, 7658, 7659, 312231};

  EXPECT_EQ(result.size(), 5ul);
  EXPECT_EQ(result, expected);
}

TEST(COPY_IF, BothConditionsMetSingleResult) {
  std::vector<int> v(100'001, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  std::vector<int> res;

  auto result = pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt == 2; });

  EXPECT_EQ(res.front(), 999);
  EXPECT_EQ(res.size(), 1);
}

TEST(COPY_IF, WholeVectorCopied) {
  std::vector<int> v(100'001, 5);


  std::vector<int> res;

  auto result = pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 4; },
      [](int i) mutable { return true; });

  EXPECT_EQ(res.size(), 100'001);
}


TEST(COPY_IF, NeitherSatisfiedReturnsEnd) {
  std::vector<int> v(100'001, 2);
  std::vector<int> res;

  auto result = pstl::copy_if(
      v.begin(), v.end(), res.begin(), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt == 1; });

  EXPECT_EQ(res.end(), result);
  EXPECT_EQ(res.size(), 0ul);
}

TEST(COPY_IF, BothAreMetExactMatchEvenLength) {
  std::vector<int> v(100'000, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[99999] = 312231;

  std::vector<int> res;

  auto result = pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [](int i) mutable { return i == 312231; });

  EXPECT_EQ(res.front(), 312231);
  EXPECT_EQ(res.size(), 1ul);
}

TEST(COPY_IF, BothConditionsMetSingleResultOddLength) {
  std::vector<int> v(100'001, 2);
  v[100000] = 312231;

  std::vector<int> res;

  auto result = pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [](int i) { return i == 312231; });

  EXPECT_EQ(res.size(), 1ul);
  EXPECT_EQ(res.front(), 312231);
}

TEST(COPY_IF, BothConditionsMetBackOfVector) {
  std::vector<int> v(100'001, 2);
  v[111] = 999;
  v[345] = 123123;
  v[100'000] = 123;

  std::vector<int> res;

  pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt == 3; });

  EXPECT_EQ(res.size(), 1);
  EXPECT_EQ(res.front(), 123);
}

TEST(COPY_IF, BothConditionsMetFrontOfVector) {
  std::vector<int> v(100'371, 2);
  v[0] = 999;

  std::vector<int> res;

  pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt == 1; });

  EXPECT_EQ(res.size(), 1);
  EXPECT_EQ(res.front(), 999);
}

TEST(COPY_IF, CopiesMultiple) {
  std::vector<int> v(100'371, 2);
  v[0] = 999;
  v[77] = 373;
  v.back() = 999;

  std::vector<int> res;

  pstl::copy_if(
      v.begin(), v.end(), std::back_inserter(res), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt >= 1; });

  std::vector<int> expected{999, 373, 999};
  EXPECT_EQ(res.size(), 3);
  EXPECT_EQ(res, expected);
}

TEST(COPY_IF, IteratorMovedCorrectly) {
  std::vector<int> v(100'371, 2);
  v[0] = 999;
  v[100'369] = 999;

  std::vector<int> res(3);

  auto next_after_copied = pstl::copy_if(
      v.begin(), v.end(), res.begin(), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt >= 1; });

  EXPECT_EQ(next_after_copied, res.end() - 1);
}

TEST(COPY_IF, IteratorMovedCorrectlyToEnd) {
  std::vector<int> v(100'371, 2);
  v[0] = 999;

  std::vector<int> res(1);

  auto next_after_copied = pstl::copy_if(
      v.begin(), v.end(), res.begin(), [](int i) { return i > 50; },
      [cnt = 0](int i) mutable { return ++cnt >= 1; });

  EXPECT_EQ(next_after_copied, res.end());
}