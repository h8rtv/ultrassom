#pragma once

#include <map>
#include <functional>
#include <memory>

#include "ISolver.hpp"

class AlgorithmFactory {
private:
  std::map<
    std::string,
    std::function<std::unique_ptr<ISolver>(const ModelMatrix&, Settings::Config)>,
    std::less<>
  > map;

  AlgorithmFactory() { };

public:
  AlgorithmFactory(AlgorithmFactory const&) = delete;
  void operator=(AlgorithmFactory const&)   = delete;

  static AlgorithmFactory& instance() {
    static AlgorithmFactory instance;
    return instance;
  }

  template<typename T>
  constexpr static bool register_algorithm(std::string name, std::function<std::unique_ptr<ISolver>(const ModelMatrix&, Settings::Config)> creator) {
    instance().map[name] = creator;
    return true;
  }

  static std::unique_ptr<ISolver>create(std::string_view key, const ModelMatrix& matrix, Settings::Config config) {
    auto& map = instance().map;
    std::unique_ptr<ISolver> pointer;
    auto found = map.find(key);
    if (found != map.end()) {
      pointer = found->second(matrix, config);
    }
    return pointer;
  }

};

#define REGISTER_ALGORITHM_DEF(_name) static bool registered_ ## _name;

#define REGISTER_ALGORITHM_IMPL(_name, _type) \
  bool _type::registered_ ## _name = AlgorithmFactory::register_algorithm<_type>( \
    #_name, \
    [](const ModelMatrix& matrix, Settings::Config config) \
    { return std::make_unique<_type>(matrix, config); } \
  );
