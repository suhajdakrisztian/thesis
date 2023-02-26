#pragma once

#include "utils.h"

#include <execution>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>

namespace parallel {

template <class ExecutionPolicy, class ForwardIter, class UnaryPredicate,
          class Predicate>
ForwardIter find_if(ExecutionPolicy&& exec, ForwardIter first,
                      ForwardIter last, UnaryPredicate pred, Predicate fn) {

  const auto element_count = std::distance(first, last);
  const auto task_count = utils::GetSubTaskCount(element_count);
  const auto step_size = element_count / task_count;

  std ::vector<std ::pair<ForwardIter, ForwardIter>> ranges(task_count);

  ForwardIter last_first = first;
  ForwardIter last_end;

  /*
  A kovetkezo reszre akartam mas implementaciot adni,de nagyon furcsan viselkedik
  ha a async-es reszben akarom advanceolni az iteratorokat, akkor neha (nem ertem miert) rossz eredmenyt ad
  */
 
  auto fst = first;
  auto lst = std::next(fst, step_size);

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
        std::advance(fst, step_size);
        std::advance(lst, step_size);
  }

  for (auto i = 0ul; i < task_count; i++) {
    tasks[i].wait();
    for (auto it = filtered_results[i].begin(); it < filtered_results[i].end(); ++it) {
      if (fn(**it)) {
        return *it;
      }
    }
  }
  return last;
}
}  // namespace pstl