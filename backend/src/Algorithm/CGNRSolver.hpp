#pragma once

#include "Factory.hpp"
#include "ISolver.hpp"

class CGNRSolver : public ISolver {
private:
  static bool registered;
public:
  Eigen::VectorXd solve(const Eigen::VectorXd& g, const Eigen::MatrixXd& H);
};
