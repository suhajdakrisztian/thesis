#include "../../src/pstl.h"

#include <numeric>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <execution>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;


enum Repetitions {THOUSAND = 1000, 
                  TEN_THOUSAND = 10'000, 
                  HUNDRED_THOUSAND = 100000, 
                  MILLION = 1'000'000, 
                  TEN_MILLION = 10'000'000, 
                  HUNDRED_MILLION = 100'000'000,
                  BILLION = 1'000'000'000};


std::pair<double, double> test_iota(size_t container_size) {


  std::vector<int> parallel_vector(container_size, 0);
  auto start_point = 50;

  auto t1 = high_resolution_clock::now();
  parallel::iota(parallel_vector.begin(), parallel_vector.end(), start_point);
  auto t2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_pstl = t2 - t1;

  std::vector<int> standard_vector(container_size, 0);

  auto t3 = high_resolution_clock::now();
  std::iota(standard_vector.begin(), standard_vector.end(), start_point);
  auto t4 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_std = t4 - t3;

  return std::make_pair(ms_double_pstl.count(), ms_double_std.count());
}


std::pair<double, double> test_copy_backward(size_t container_size) {
  
  std::vector<std::vector<int>> parallel_vector(container_size);
  std::vector<std::vector<int>> standard_vector(container_size);

  std::vector<std::vector<int>> expected;

  for(int i = 0; i < container_size; i++) {
    expected.push_back(std::vector<int>(1000));
  }

  auto t1 = high_resolution_clock::now();
  pstl::copy_backward(expected.begin(), expected.end(), parallel_vector.end());
  auto t2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_pstl = t2 - t1;

  auto t3 = high_resolution_clock::now();
  std::copy_backward(expected.begin(), expected.end(), standard_vector.end());
  auto t4 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_std = t4 - t3;

  return std::make_pair(ms_double_pstl.count(), ms_double_std.count());
}

std::pair<double, double> test_find_if() {

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

  auto t1 = high_resolution_clock::now();

  auto res = parallel::find_if(
       v.begin(), v.end(), [](int i) { return i > 50; },
      [](int i)  { return true; });

  auto t2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_pstl = t2 - t1;

  auto t3 = high_resolution_clock::now();

  auto res2 = std::find_if(std::execution::par,
       v.begin(), v.end(), [](int i) { return i > 50; });

  auto t4 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_std = t4 - t3;

  return std::make_pair(ms_double_pstl.count(), ms_double_std.count());

}


std::pair<double, double> test_copy_if(){ 

  std::vector<int> v(100'000, 2);
  std::vector<int> dest(100'000);
  
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  std::vector<int> v2(100'000, 2);
  std::vector<int> tmp2(100'000);
  std::vector<int> dest2(100'000);

  
  v[5001] = 123;
  v[11001] = 999;
  v[22222] = 1233;
  v[33333] = 546464;
  v[55555] = 555;
  v[77777] = 7657;
  v[77778] = 7658;
  v[77779] = 7659;
  v[88888] = 312231;

  auto t1 = high_resolution_clock::now();

  auto res = pstl::copy_if(
       v.begin(), v.end(), dest.begin(),
       [](int i) { return i > 50; },
      [cnt = 5](int i) mutable { return ++cnt == 5; });

  auto t2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_pstl = t2 - t1;

  auto t3 = high_resolution_clock::now();

  auto res2 = std::copy_if(std::execution::par,
       v.begin(), v.end(), tmp2.begin(),
       [](int i) { return i > 50; });
  
  auto res3 = std::copy_if(std::execution::par, tmp2.begin(), tmp2.end(), dest2.begin(),
      [cnt = 0](int i) mutable { return ++cnt == 5; });

  auto t4 = high_resolution_clock::now();

  duration<double, std::milli> ms_double_std = t4 - t3;

  return std::make_pair(ms_double_pstl.count(), ms_double_std.count());

 }


int not_main() {

  std::ofstream MyFile("copy_if_results.csv");
  MyFile << "PSTL,STD" << std::endl;

  //test_iota(Repetitions::HUNDRED_MILLION);
  //test_copy_backward(Repetitions::THOUSAND);

  for (size_t i = 0; i < Repetitions::THOUSAND; i++)
  {
    auto result = test_copy_if();
    MyFile << result.first << "," << result.second << std::endl;
  }
  MyFile.close();
  
  return 0;
}

