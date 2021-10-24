#pragma once 

#include <Eigen/Dense>

class ISolver {
public:
  virtual Eigen::VectorXd solve(const Eigen::VectorXd& g, const Eigen::MatrixXd& H) = 0; 
};
