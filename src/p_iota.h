#pragma once

#include <execution>
#include <thread>
#include <iterator>
#include <future>

namespace parallel {
    template<class ExecutionPolicy, class ForwardIter>
    void iota(ExecutionPolicy &&exec, ForwardIter first, ForwardIter last, auto n) {

      const auto chunks = std::thread::hardware_concurrency();
      const unsigned int dist = std::distance(last, first);
      const auto chunkLen = dist / chunks;

      std::vector<std::future<void>> tasks(chunks);

      auto current_begin = first;
      for(size_t i = 0; i < chunks; ++ i) {

        auto startIt = std::next(current_begin, i * chunkLen);
        auto endIt = std::min(last, std::next(startIt, chunkLen));

        tasks[i] = std::async(std::launch::async, [&] {
            std::iota(startIt, endIt,n);
        });
      }
      for(auto& task: tasks) {
        task.wait();
      }
    }
} // namespace

