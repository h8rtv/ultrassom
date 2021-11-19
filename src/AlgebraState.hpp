#pragma once

#include <Eigen/Dense>
#include "Parser/CSVFileParser.hpp"

class AlgebraState {
private:
  Eigen::MatrixXd modelMatrix;

  AlgebraState() {
    Eigen::initParallel();
    modelMatrix = CSVFileParser(MODEL_MATRIX).parse();
  }

public:
  AlgebraState(AlgebraState const&)    = delete;
  void operator=(AlgebraState const&)  = delete;

  [[nodiscard]]
  const Eigen::MatrixXd& get_model_matrix() const {
    return modelMatrix;
  }

  [[nodiscard]]
  static const AlgebraState& instance() {
    static AlgebraState instance;
    
    return instance;
  }

};
