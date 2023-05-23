#pragma once

#include "Utils.h"

#include <execution>
#include <future>
#include <thread>
#include <iterator>
#include <vector>
#include <iostream>

namespace pstl {
    template<class ForwardIter, class ValueType> requires std::forward_iterator<ForwardIter> && std::integral<ValueType>
    void iota(ForwardIter first, ForwardIter last, ValueType value) {

        const auto element_count = static_cast<size_t>(std::distance(first, last));
        const auto task_count = element_count < 10'000'000 ? 1: element_count / 10'000'000;
        const auto step_size = element_count / task_count;

      std::vector<std::pair<ForwardIter, ForwardIter>> ranges(task_count);
      std::vector<ValueType> numbers(task_count);

      for(auto i = 0; i < task_count; ++ i) {
        numbers[i] = i ? numbers[i - 1] + step_size : value;
        ranges[i].first = i ? first + i * step_size : first;
        ranges[i].second = i + 1 == task_count ? last : first + i * step_size + step_size;
      }

      std::vector<std::future<void>> futures(task_count);

      for(auto i = 0; i < task_count; ++ i) {
        futures[i] = std::async(std::launch::async, [&, value = numbers[i], i]() mutable {
            for(auto it = ranges[i].first; it < ranges[i].second; it ++) {
              *it = value++;
            }
        });
      }

      for(auto &future: futures) {
        future.wait();
      }
    }
}  // namespace pstl