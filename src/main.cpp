#include <vector>
#include "pstl_find_if.h"
#include "pstl_iota.h"
#include <chrono>
#include <cassert>

int main() {
/*
  std::vector<int> v(100, 2);
  v[5] = 123;
  v[12] = 999;
  v[17] = 1233;
  v[23] = 546464;
  v[45] = 555;
  v[66] = 7657;
  v[77] = 7658;
  v[78] = 7659;
  v[79] = 7655;
  v[93] = 312231;

  auto res = parallel::find_if(
          std::execution::par,
          v.begin(),
          v.end(),
          [](int i) { return i > 50; },
          [counter = 0](int i) mutable { return ++ counter >= 5; });

  std::cout << *res << std::endl;*/

  //std::vector<int> benchmark {- 10, - 9, - 8, - 7, - 6, - 5, - 4, - 3, - 2, - 1};
  //std::vector<int> vec(15);
  //parallel::iota(vec.begin(), vec.end(), -20);
  //assert(benchmark == vec);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration;

  auto tl1 = high_resolution_clock::now();
  std::vector<int> linear(200'000);
  std::iota(linear.begin(), linear.end(), 1);
  auto tl2 = high_resolution_clock::now();

  auto ls_int = duration_cast<std::chrono::milliseconds>(tl2 - tl1);
  linear.clear();

  auto tp1 = high_resolution_clock::now();
  std::vector<int> vec(200'000);
  parallel::iota(vec.begin(), vec.end(), 1);
  auto tp2 = high_resolution_clock::now();

  auto ps_int = duration_cast<std::chrono::milliseconds>(tp2 - tp1);

  std::cout << ls_int.count() << std::endl << ps_int.count();

  //assert(vec.size() == 100);
  //assert(vec.front() == 1);
  //assert(vec.back() == 100);
  //assert(std::accumulate(vec.begin(), vec.end(), 0) == 5050);

  return 0;
}
