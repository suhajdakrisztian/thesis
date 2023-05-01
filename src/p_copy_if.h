#pragma once

#include "iterator.h"

#include <thread>
#include <vector>
#include <future>

namespace pstl {

    template<typename ForwardIter,
            typename OutputIter,
            typename UnaryPredicate,
            typename Predicate>
            requires std::forward_iterator<ForwardIter> && std::output_iterator<OutputIter, typename std::iterator_traits<ForwardIter>::value_type> &&
            std::predicate<UnaryPredicate, typename std::iterator_traits<ForwardIter>::value_type>
            && std::same_as<std::invoke_result_t<UnaryPredicate, typename std::iterator_traits<ForwardIter>::value_type>, bool>

    OutputIter copy_if(ForwardIter first,
                       ForwardIter last,
                       OutputIter dest,
                       UnaryPredicate pred,
                       Predicate fn) {

        const auto element_count = static_cast<size_t>(std::distance(first, last));
        const auto cores = static_cast<size_t>(std::thread::hardware_concurrency());
        const auto task_count = element_count < 100000 ? 1 : element_count / 50000;
        const auto step_size = element_count / task_count;

        std::vector<std::pair<ForwardIter, ForwardIter>> ranges(task_count);

        ForwardIter last_first = first;
        ForwardIter last_end;

        for(auto i = 0ul; i < task_count; ++ i) {
            ranges[i].first = last_first;
            std::advance(last_first, step_size);
            ranges[i].second = last_end = last_first;
        }

        ranges.back().second = last;

        std::vector<std::vector<ForwardIter>> filtered_results(task_count);
        std::vector<std::shared_future<void>> tasks(task_count);

        for(auto i = 0ul; i < task_count; i ++) {
            tasks[i] = std::async(std::launch::async,
                                  [&filtered_results, &ranges, i, &pred]() {
                                      for(auto it = ranges[i].first; it < ranges[i].second; ++ it) {
                                          if(pred(*it)) {
                                              filtered_results[i].push_back(it);
                                          }
                                      }
                                  });
        }

        OutputIter result = dest;

        for(auto& task: tasks) {
            task.wait();
        }

        LambdaFilter filtered(filtered_results, fn);

        for(auto iter = filtered.begin(); iter < filtered.end(); ++ iter) {
            *(result++) = **iter;
        }
/*
        for(auto i = 0ul; i < task_count; i ++) {
            tasks[i].wait();
            for(auto it = filtered_results[i].begin(); it < filtered_results[i].end(); ++ it) {
                if(fn(**it)) {
                    *(result++) = **it;
                }
            }
        }*/
        return result;
    }
}