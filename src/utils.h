#pragma once

#include <thread>

namespace utils {

constexpr std::size_t _multiplier = 32;
template <class _Diff>
constexpr std::size_t GetSubTaskCount(const _Diff _Count) {
  const auto _sizeCount = static_cast<std ::size_t>(_Count);
  const auto _coreCount =
      static_cast<std ::size_t>(std::thread::hardware_concurrency());
  return std::min(_coreCount * _multiplier, _sizeCount);
}

}  // namespace utils