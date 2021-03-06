#pragma once

#include "Factory.hpp"
#include "ISolver.hpp"

class CGNESolver : public ISolver {
private:
  REGISTER_ALGORITHM_DEF(CGNE);
public:
  CGNESolver(const ModelMatrix& modelMatrix, Settings::Config config = Settings::Config())
  : ISolver(modelMatrix, config) {};
  std::pair<Eigen::VectorXd, uint> solve(const Eigen::VectorXd& g);
};
