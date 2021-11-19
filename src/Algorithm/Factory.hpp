#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include "ISolver.hpp"

#define REGISTER_ALGORITHM (type, name)            \
  AlgorithmFactory::register_algorithm<type>(name, \
  []() {                                           \
    return std::unique_ptr<T>(new T))              \
  }

class AlgorithmFactory {
private:
  std::unordered_map<std::string, std::function<std::unique_ptr<ISolver>()>> map;

  AlgorithmFactory() { };

public:
  AlgorithmFactory(AlgorithmFactory const&) = delete;
  void operator=(AlgorithmFactory const&)   = delete;

  static AlgorithmFactory& instance() {
    static AlgorithmFactory instance;
    return instance;
  }

  template<typename T>
  static void register_algorithm(std::string name, std::function<std::unique_ptr<ISolver>()> creator) {
    instance().map[name] = creator;
  }

  static std::unique_ptr<ISolver>create(const std::string& key) {
    auto& map = instance().map;
    std::unique_ptr<ISolver> pointer;
    if (map.count(key)) {
      pointer = (map[key])();
    }
    return pointer;
  }

};
