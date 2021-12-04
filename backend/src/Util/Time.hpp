#pragma once

#include <chrono>
#include <iomanip>
#include <functional>

namespace Util::Time {
  using fsec = std::chrono::duration<float>;
  using TimeInfo = std::pair<float, std::string>;

  std::string toString(std::chrono::system_clock::time_point time) {
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%FT%T%z");
    return ss.str();
  }

  [[nodiscard]]
  std::pair<std::string, std::function<TimeInfo()>> time_it() {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = [start]() {
      auto end = std::chrono::high_resolution_clock::now();
      fsec duration = end - start;
      return std::make_pair(duration.count(), toString(end));
    };

    return {
      toString(start),
      finish
    };
  }
}