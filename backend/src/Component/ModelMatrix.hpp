#pragma once

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
    OATPP_LOGI("ModelMatrix", "Start parse");
    auto matrices = std::make_shared<ModelMatrix>();
    matrices->H = CSVFileParser(MODEL_MATRIX).parse();
    OATPP_LOGI("ModelMatrix", "Finish parse");
    OATPP_LOGI("ModelMatrix", "Start computing transposition");
    matrices->Ht = matrices->H.transpose();
    OATPP_LOGI("ModelMatrix", "Finish computing transposition");
    return matrices;
  }());
};
