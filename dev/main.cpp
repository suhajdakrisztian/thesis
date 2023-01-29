#include <functional>
#include <iostream>
#include <set>
#include <vector>


#define ARBITRARY_THRESHOLD 100'000

namespace parallel_algorithms {

template <typename T>
std::vector<std::vector<T>> split_vector(const std::vector<T>& vec) {
  std::vector<std::vector<T>> outVec;
  std::unordered_set<std::future<Iterator>> results;

  size_t length = vec.size() / ARBITRARY_THRESHOLD;
  size_t remain = vec.size() % ARBITRARY_THRESHOLD;

  size_t begin = 0;
  size_t end = 0;

  for (size_t i = 0; i < std::min(ARBITRARY_THRESHOLD, vec.size()); ++i) {
    end += (remain > 0) ? (length + !!(remain--)) : length;

    //outVec.push_back(std::vector<T>(vec.begin() + begin, vec.begin() + end));

    begin = end;
  }

  return outVec;
}

template <typename... Args>
auto fold(Args... args) {
  return (args.get() + ...);
}

template <class Type, class ForwardIterator>
std::vector<Type> parallel_copy_if(ForwardIterator begin, ForwardIterator end,
                                   ForwardIterator destination_begin,
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