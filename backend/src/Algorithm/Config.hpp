#pragma once

struct Config {
  uint maxIterations;
  double maxError;
};

enum Quality {
  LOW,
  MEDIUM,
  HIGH,
};

constexpr Config configs[] = {
  { 20, 1e-4 },
  { 50, 1e-6 },
  { 100, 1e-8 },
};

constexpr Config get_config(Quality quality) {
  return configs[quality];
}
