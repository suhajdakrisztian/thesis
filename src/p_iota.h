#pragma once

#include "Utils.h"

#include <execution>
#include <future>
#include <thread>
#include <iterator>
#include <vector>

namespace parallel {

    template<class ExecutionPolicy, class ForwardIter, class Value>
    void iota(ExecutionPolicy &&exec, ForwardIter first,
              ForwardIter last, Value v) {

      const auto element_count = std::distance(first, last);
      const auto task_count = 7; //this should be refined to some actual calculation
      const auto step_size = element_count / task_count;

      std::vector<std::pair<ForwardIter, ForwardIter>> ranges(task_count);
      std::vector<Value> numbers(task_count);

      ForwardIter last_first = first;
      ForwardIter last_end;

      for(auto i = 0ul; i < task_count; ++ i) {
        numbers[i] = i ? numbers[i - 1] + step_size : v;
        ranges[i].first = i ? first + i * step_size : first;
        ranges[i].second = i + 1 == task_count ? last : first + i * step_size + step_size;
      }
      //ranges.back().second = last; //this needed for unequal ranges

      std::vector<std::shared_future<void>> tasks(task_count);

      for(auto i = 0ul; i < task_count; i ++) {
        tasks[i] = std::async(std::launch::async,
                              [&]() {
                                  std::iota(ranges[i].first, ranges[i].second, numbers[i]);
                              });
      }
      for(auto &task: tasks) { task.wait(); }
    }
}  // namespace pstl