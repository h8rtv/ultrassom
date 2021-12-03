#pragma once

namespace Settings {
  struct Config {
    uint maxIterations;
    double maxError;
  };

  enum Quality {
    LOW,
    MEDIUM,
    HIGH,
  };

  constexpr Settings::Config configs[] = {
    { 20, 1e-4 },
    { 50, 1e-6 },
    { 100, 1e-8 },
  };

  constexpr Settings::Config setting(Quality quality) {
    return configs[quality];
  }
}
