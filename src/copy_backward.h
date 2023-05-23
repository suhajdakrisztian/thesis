#pragma once

#include<future>
#include <iostream>
#include<vector>
#include<thread>

namespace pstl {
    template<class BidirectionalIter> requires std::bidirectional_iterator<BidirectionalIter>
    BidirectionalIter copy_backward(BidirectionalIter first, BidirectionalIter last, BidirectionalIter dest) {

        const auto element_count = static_cast<size_t>(std::distance(first, last));
        const auto task_count = element_count < 100'000 ? 1 : element_count / 50'000;
        const auto step_size = element_count / task_count;

        std::vector<std::pair<BidirectionalIter, BidirectionalIter>> ranges(task_count);
        std::vector<BidirectionalIter> starting_points(task_count);

        auto starting_point = dest;
        auto left_end_of_window = first;
        BidirectionalIter right_end_of_window;

        for(auto i = 0ul; i < task_count; ++ i) {
            ranges[i].first = left_end_of_window;
            std::advance(left_end_of_window, step_size);

            ranges[i].second = right_end_of_window = left_end_of_window;

            starting_points[task_count - i - 1] = starting_point;
            std::advance(starting_point, -step_size);
        }


        ranges.back().second = last;

        std::vector<std::future<void>> futures(task_count);

        for(auto i = 0; i < task_count; ++ i) {
            futures[i] = std::async(std::launch::async, [&, i]() mutable {

                auto first = ranges[i].first;
                auto last = ranges[i].second;

                while(first != last) {
                    *(--starting_points[i]) = *(--last);
                }
            });
        }
        for(auto &future: futures) {
            future.wait();
        }
        return ranges.front().first;
    }
} //namespace
