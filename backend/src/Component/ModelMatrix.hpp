#pragma once

#include <iostream>
#include <Eigen/Dense>

#include "ModelMatrix.hpp"
#include "Parser/CSVFileParser.hpp"

class ModelMatrixComponent {
public:

  /**
   *  Create Model Matrix Component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<ModelMatrix>, modelMatrix)([] {
    Eigen::initParallel();
    std::cout << "Start parse Model Matrix" << std::endl;
    auto matrices = std::make_shared<ModelMatrix>();
    matrices->H = CSVFileParser(MODEL_MATRIX).parse();
    std::cout << "Finish parse Model Matrix" << std::endl;
    std::cout << "Start compute transposition" << std::endl;
    matrices->Ht = matrices->H.transpose();
    std::cout << "Finish Compute transposition" << std::endl;
    return matrices;
  }());
};
