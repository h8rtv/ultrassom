#pragma once

#include <Eigen/Dense>

#include "Parser/CSVFileParser.hpp"

class ModelMatrix {
public:

  /**
   *  Create Model Matrix Component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<Eigen::MatrixXd>, modelMatrix)([] {
    Eigen::initParallel();
    return std::make_shared<Eigen::MatrixXd>(CSVFileParser(MODEL_MATRIX).parse());
  }());
};
