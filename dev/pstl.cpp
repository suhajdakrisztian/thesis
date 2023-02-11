#include <execution>
#include <future>
#include <iterator>
#include <vector>
#include <iostream>

namespace utils {

constexpr std::size_t _multiplier = 32;
template <class _Diff>
constexpr std::size_t GetSubTaskCount(const _Diff _Count) {
  const auto _sizeCount = static_cast<std ::size_t>(_Count);
  const auto _coreCount = static_cast<std ::size_t>(std::thread::hardware_concurrency());
  return std::min(_coreCount * _multiplier, _sizeCount);
}
}  // namespace utils

template <class ExecutionPolicy, class ForwardIter, class UnaryPredicate, class Predicate>
ForwardIter find_if(ExecutionPolicy&& exec, ForwardIter first, ForwardIter last, UnaryPredicate pred, Predicate fn) {
  size_t dist = std ::distance(first, last);
  size_t chunks = utils::GetSubTaskCount(dist);

  std ::vector<std ::pair<ForwardIter, ForwardIter>> ranges(chunks);

  ForwardIter last_first = first;
  ForwardIter last_end;

  for (auto i = 0ul; i < chunks; ++i) {
    ranges[i].first = last_first;
    std ::advance(last_first, dist / chunks);
    ranges[i].second = last_end = last_first;
  }

  std ::vector<std ::vector<ForwardIter>> filtered_results(chunks);
  std ::vector<std ::shared_future<void>> futures(chunks);

  for (int i = 0; i < chunks; ++i) {
    std ::future<void> f = std ::async(std ::launch ::async, [&filtered_results, &ranges, i, &pred]() {
          for (auto it = ranges[i].first; it != ranges[i].second; ++it) {
            if (pred(*it)) {
              filtered_results[i].push_back(it);
            }
          }
        });
    futures[i] = f.share();
  }

  ForwardIter result = last;

  for (int i = 0; i < chunks && result == last; ++i) {
    futures[i].wait();

    for (auto it = filtered_results[i].begin();
         it != filtered_results[i].end() && result == last; ++it) {
      if (fn(**it)) {
        result = *it;
      }
    }
  }

  return result;
}

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

  auto res = find_if(
      std::execution::par, v.begin(), v.end(), 
      [](int i) { return i > 50; },
      [counter = 0](int i) mutable {return ++counter == 5;});

      std::cout << *res << '\n';

  return 0;
}