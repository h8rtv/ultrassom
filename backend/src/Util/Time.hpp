#pragma once

#include <chrono>
#include <functional>

namespace Util::Time {
  using fsec = std::chrono::duration<float>;
  [[nodiscard]]
  std::function<float()> time_it() {
    auto start = std::chrono::high_resolution_clock::now();
    return [start]() {
      auto end = std::chrono::high_resolution_clock::now();
      fsec duration = end - start;
      return duration.count();
    };
  }
}