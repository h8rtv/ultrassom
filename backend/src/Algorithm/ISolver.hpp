#pragma once 

#include <Eigen/Dense>
#include "ModelMatrix.hpp"
#include "Settings.hpp"

class ISolver {
protected:
  const ModelMatrix& modelMatrix;
  Settings::Config config;
  ISolver(const ModelMatrix& matrix, Settings::Config config = Settings::Config())
  : modelMatrix(matrix),
    config(config)
  {};
public:
  virtual std::pair<Eigen::VectorXd, uint> solve(const Eigen::VectorXd& g) = 0; 
};
