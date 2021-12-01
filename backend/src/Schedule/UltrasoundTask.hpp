#pragma once

#include <string_view>

#include "ModelMatrix.hpp"
#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageGeneration.hpp"

class UltrasoundTask {
private:
  v_int32 user;
  std::string algorithm;
  std::string data;
  const ModelMatrix& modelMatrix;

public:
  UltrasoundTask(
    v_int32 user,
    std::string algorithm,
    std::string data,
    const ModelMatrix& modelMatrix)
  : user(user),
    algorithm(algorithm),
    data(data),
    modelMatrix(modelMatrix)
  {};

  void operator()() const {
    auto solver = AlgorithmFactory::create(algorithm, modelMatrix);
    Eigen::VectorXd g = CSVParser(data).parse();
    Eigen::VectorXd f = solver->solve(g);
    std::string img = gen_image(f);
  };
};
