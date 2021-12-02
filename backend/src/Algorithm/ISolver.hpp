#pragma once 

#include <Eigen/Dense>
#include "ModelMatrix.hpp"
#include "Config.hpp"

class ISolver {
protected:
  const ModelMatrix& modelMatrix;
  Config config;
  ISolver(const ModelMatrix& matrix, Config config = Config())
  : modelMatrix(matrix),
    config(config)
  {};
public:
  virtual Eigen::VectorXd solve(const Eigen::VectorXd& g) = 0; 
};
