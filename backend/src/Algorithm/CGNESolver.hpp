#pragma once

#include "Factory.hpp"
#include "ISolver.hpp"

class CGNESolver : public ISolver {
private:
  static bool registered;
public:
  CGNESolver(const ModelMatrix& modelMatrix, Settings::Config config = Settings::Config())
  : ISolver(modelMatrix, config) {};
  Eigen::VectorXd solve(const Eigen::VectorXd& g);
};
