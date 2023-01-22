#include <bits/stdc++.h>

namespace parallel_algorithms {

#if __linux__
constexpr auto PARALLEL_THRESHOLD = 250'000ULL;
#elif __ANDROID__
constexpr auto PARALLEL_THRESHOLD = 100'000ULL;
#elif __APPLE__
constexpr auto PARALLEL_THRESHOLD = 180'000ULL;
#elif _WIN32
constexpr auto PARALLEL_THRESHOLD = 10'000ULL;
#else
constexpr auto PARALLEL_THRESHOLD = 100'000ULL;
#endif

template <typename... Args>
auto fold(Args... args) {
  return (args.get() + ...);
}

template <class Type, class Iterator>
std::vector<Type> parallel_copy_if(Iterator begin, Iterator end,
                                   Iterator destination_begin,
                                   const std::function<bool>& func) {
  const auto partition_count = (end - begin) / PARALLEL_THRESHOLD;
  std::unordered_set<std::future<Iterator>> results;

  if (1 == partition_count) {
    return std::copy_if(begin, end, destination_begin, func);
  }

  for (auto partition = 1ull; partition <= partition_count; partition++) {
    const auto current_limit = (partition / partition_count);

    results.insert(std::async(std::copy_if(
        begin * current_limit, begin + (end - begin) * current_limit,
        destination_begin, func)));
  }

  return fold(destination_begin);
}
}  // namespace parallel_algorithms

int main() {
  std::cout << "szakdoga" << std::endl;
  return 0;
}