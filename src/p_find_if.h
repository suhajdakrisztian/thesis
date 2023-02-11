#pragma once

#include <execution>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>

namespace utils {

constexpr std::size_t _multiplier = 32;
template <class _Diff>
constexpr std::size_t GetSubTaskCount(const _Diff _Count) {
  const auto _sizeCount = static_cast<std ::size_t>(_Count);
  const auto _coreCount = static_cast<std ::size_t>(std::thread::hardware_concurrency());
  return std::min(_coreCount * _multiplier, _sizeCount);
}
}  // namespace utils

namespace pstl {

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

  for (auto i = 0ul; i < chunks; i++) {
    std ::future<void> f = std ::async(
        std ::launch ::async, [&filtered_results, &ranges, i, &pred]() {
          for (auto it = ranges[i].first; it != ranges[i].second; ++it) {
            if (pred(*it)) {
              filtered_results[i].push_back(it);
            }
          }
        });
    futures[i] = f.share();
  }

  auto result = last;

  for (auto i = 0ul; i < chunks && result == last; i++) {
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
} //namespace pstl