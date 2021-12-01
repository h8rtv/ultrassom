#pragma once 

#include <Eigen/Dense>
#include "ModelMatrix.hpp"

class ISolver {
protected:
  const ModelMatrix& modelMatrix;
  ISolver(const ModelMatrix& matrix) : modelMatrix(matrix) {}
public:
  virtual Eigen::VectorXd solve(const Eigen::VectorXd& g) = 0; 
};
