#pragma once

#include<future>
#include <iostream>
#include<vector>
#include<thread>

namespace pstl {

    template<class BidirectionalIter>
    BidirectionalIter copy_backward(BidirectionalIter begin, BidirectionalIter end, BidirectionalIter dest) {

        const auto element_count = std::distance(begin, end);
        const auto task_count = 2;
        const auto step_size = element_count / task_count;

        std::vector<std::pair<BidirectionalIter, BidirectionalIter>> ranges(task_count);
        std::vector<BidirectionalIter> starting_points(task_count);

        auto starting_point = dest;
        auto last_first = begin;
        BidirectionalIter last_end;

        for(auto i = 0ul; i < task_count; ++ i) {
            ranges[i].first = last_first;
            std::advance(last_first, step_size);
            ranges[i].second = last_end = last_first;
            starting_points[task_count - i - 1] = starting_point;
            starting_point -= step_size;
        }

        //pstl::FuturePool future_pool{};

        std::vector<std::future<void>> futures(task_count);

        for(auto i = 0; i < task_count; ++ i) {
            futures[i] = std::async(std::launch::async, [&, i]() mutable {

                auto first = ranges[i].first;
                auto last = ranges[i].second;

                while(first != last) {
                    *(--starting_points[i]) = *(--last);
                }
            });

            //future_pool.submit_task(future);
        }

        for(auto &future: futures) {
            future.wait();
        }

        //future_pool.await();
        return ranges.front().first;
    }

} //namespace
