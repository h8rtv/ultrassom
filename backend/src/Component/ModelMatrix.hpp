#pragma once

#include <iostream>
#include <Eigen/Dense>

#include "Parser/CSVFileParser.hpp"

class ModelMatrix {
public:

  /**
   *  Create Model Matrix Component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<Eigen::MatrixXd>, modelMatrix)([] {
    Eigen::initParallel();
    std::cout << "Start parse Model Matrix" << std::endl;
    auto H = std::make_shared<Eigen::MatrixXd>(CSVFileParser(MODEL_MATRIX).parse());
    std::cout << "Finish parse Model Matrix" << std::endl;
    return H;
  }());
};
