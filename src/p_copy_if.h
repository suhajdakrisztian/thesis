#pragma once

#include "utils.h"

#include <execution>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>

namespace parallel {

template <class ForwardIter> class PSTL {
private:
  std::vector<std::vector<ForwardIter>> _filtered_results;
  std::vector<std::vector<ForwardIter>> _final_results;
  std::vector<std::shared_future<void>> _tasks;

public:
  PSTL() = default;
  Iterator begin() { return Iterator(*_final_results.front().begin()); }
  Iterator end() { return Iterator(*_final_results.back().end()); }

  template <class ExecutionPolicy, class OutputIter, class UnaryPredicate, class Predicate>
  OutputIter copy_if(ExecutionPolicy &&exec, ForwardIter first,
                      ForwardIter last, OutputIter dest, UnaryPredicate pred, Predicate fn) {

    const auto element_count = std::distance(first, last);
    const auto task_count = utils::GetSubTaskCount(element_count);
    const auto step_size = element_count / task_count;

    std::vector<std::pair<ForwardIter, ForwardIter>> ranges(task_count);

    ForwardIter last_first = first;
    ForwardIter last_end;

    for (auto i = 0ul; i < task_count; ++i) {
      ranges[i].first = last_first;
      std::advance(last_first, step_size);
      ranges[i].second = last_end = last_first;
    }

    _filtered_results.resize(task_count);
    _tasks.resize(task_count);
    _final_results.resize(task_count);

    for (auto i = 0ul; i < task_count; i++) {
      _tasks[i] = std::async(std::launch::async, [this, &ranges, i, &pred]() {
        for (auto it = ranges[i].first; it < ranges[i].second; ++it) {
          if (pred(*it)) {
            _filtered_results[i].push_back(it);
          }
        }
      });
    }

    for (auto i = 0ul; i < task_count; i++) {
      _tasks[i].wait();
      for (auto it = _filtered_results[i].begin(); it < _filtered_results[i].end(); ++it) {
        if (fn(**it)) {
          *dest = **it;
          dest++;
          ;_final_results[i].push_back(*it);
        }
      }
    }
    return dest;
  }
};

} // namespace parallel