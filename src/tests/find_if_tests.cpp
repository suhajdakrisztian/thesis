#include "../../src/p_find_if.h"

#include <gtest/gtest.h>

TEST(FIND_IF, ReturnsCorrectIterator) {

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

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(555, *res);
}

TEST(FIND_IF, ReturnsEndWhenNoMatchForSecondPredicate) {

  std::vector<int> v(100'000, 100);

  auto res = parallel::find_if(
      v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 100'000'000; });

  EXPECT_EQ(v.end(), res);
}

TEST(FIND_IF, FiltersCorrectlyForEvenNumbers) {

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

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i) mutable { return  i % 2 == 0; });

  EXPECT_EQ(546464, *res);
}

TEST(FIND_IF, ReturnsCorrectNumberOddlength) {

  std::vector<int> v(100'911, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(555, *res);
}

TEST(FIND_IF, ReturnsEndWhenNoElementOddLength) {

  std::vector<int> v(100'777, 2);

  auto res = parallel::find_if(
      v.begin(), v.end(), [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  EXPECT_EQ(v.end(), res);
}

TEST(FIND_IF, FiltersCorrectlyOddLength) {

  std::vector<int> v(100'387, 2);
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i) mutable { return  i % 2 == 0; });

  EXPECT_EQ(546464, *res);
}

TEST(FIND_IF, OnlyMatchForFristPred) {

  std::vector<int> v(100'333, 2);
  v[111] = 2001;
  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i) mutable { return  i % 2 == 0; });

  EXPECT_EQ(v.end(), res);
}

TEST(FIND_IF, FindsFirst) {

  std::vector<int> v(100'333, 2);
  v[0] = 2001;

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i) mutable { return  i % 2; });

  EXPECT_EQ(v.begin(), res);
}

TEST(FIND_IF, FindsLast) {

  std::vector<int> v(100'333, 2);
  v.back() = 2001;

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i) mutable { return  i % 2 != 0; });

  EXPECT_EQ(v.end() - 1, res);
}


