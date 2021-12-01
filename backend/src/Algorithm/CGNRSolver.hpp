#pragma once

#include "Factory.hpp"
#include "ISolver.hpp"

class CGNRSolver : public ISolver {
private:
  static bool registered;
public:
  CGNRSolver(const ModelMatrix& modelMatrix) : ISolver(modelMatrix) {};
  Eigen::VectorXd solve(const Eigen::VectorXd& g);
};
