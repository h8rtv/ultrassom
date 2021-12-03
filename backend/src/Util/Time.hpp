#pragma once

#include <chrono>
#include <iomanip>
#include <functional>

namespace Util::Time {
  using fsec = std::chrono::duration<float>;
  using TimeInfo = std::tuple<float, std::string, std::string>;

  std::string toString(std::chrono::system_clock::time_point time) {
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%FT%T%z");
    return ss.str();
  }

  [[nodiscard]]
  std::function<TimeInfo()> time_it() {
    auto start = std::chrono::high_resolution_clock::now();
    return [start]() {
      auto end = std::chrono::high_resolution_clock::now();
      fsec duration = end - start;
      return std::make_tuple(duration.count(), toString(start), toString(end));
    };
  }
}