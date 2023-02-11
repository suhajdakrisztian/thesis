#include "p_find_if.h"

#include <vector>
#include <iostream>
#include <execution>

int main() {
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
      std::execution::par, v.begin(), v.end(), 
      [](int i) { return i > 50; },
      [counter = 0](int i) mutable { return ++counter == 5; });

  std::cout << *res << '\n';
  int i = 0;
  return 0;
}