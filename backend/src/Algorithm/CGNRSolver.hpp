#pragma once

#include "Factory.hpp"
#include "ISolver.hpp"

class CGNRSolver : public ISolver {
private:
  REGISTER_ALGORITHM_DEF(CGNR);
public:
  CGNRSolver(const ModelMatrix& modelMatrix, Settings::Config config = Settings::Config())
  : ISolver(modelMatrix, config) {};
  std::pair<Eigen::VectorXd, uint> solve(const Eigen::VectorXd& g);
};
