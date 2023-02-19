#pragma once

#include "utils.h"

#include <execution>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>

namespace parallel {

template <class ExecutionPolicy, class ForwardIter, class OutputIter, class UnaryPredicate,
          class Predicate>
ForwardIter copy_if(ExecutionPolicy&& exec, ForwardIter first,
                      ForwardIter last, OutputIter dest, UnaryPredicate pred, Predicate fn) {

  const auto element_count = std::distance(first, last);
  const auto task_count = utils::GetSubTaskCount(element_count);
  const auto step_size = element_count / task_count;

  std ::vector<std ::pair<ForwardIter, ForwardIter>> ranges(task_count);

  ForwardIter last_first = first;
  ForwardIter last_end;

  for (auto i = 0ul; i < task_count; ++i) {
    ranges[i].first = last_first;
    std ::advance(last_first, step_size);
    ranges[i].second = last_end = last_first;
  } 

  std::vector<std::vector<ForwardIter>> filtered_results(task_count);
  std::vector<std::shared_future<void>> tasks(task_count);

  for (auto i = 0ul; i < task_count; i++) {

    tasks[i] = std::async(std::launch::async,
        [&filtered_results, &ranges,i, &pred]() {
          for (auto it = ranges[i].first; it < ranges[i].second; ++it) {
            if (pred(*it)) {
              filtered_results[i].push_back(it);
            }
          }
        });
  }

  for (auto i = 0ul; i < task_count; i++) {
    tasks[i].wait();
    for (auto it = filtered_results[i].begin(); it < filtered_results[i].end(); ++it) {
      if (fn(**it)) {
        *dest = **it;
        dest++;
      }
    }
  }
  return dest;
}

} // parallel