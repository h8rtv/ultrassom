#pragma once

#include <string_view>
#include <Eigen/Dense>

#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageGeneration.hpp"

class UltrasoundTask {
private:
  v_int32 user;
  std::string algorithm;
  std::string data;
  const Eigen::MatrixXd& H;

public:
  UltrasoundTask(
    v_int32 user,
    std::string algorithm,
    std::string data,
    const Eigen::MatrixXd& H)
  : user(user),
    algorithm(algorithm),
    data(data),
    H(H)
  {};

  void operator()() const {
    auto solver = AlgorithmFactory::create(algorithm);
    Eigen::VectorXd g = CSVParser(data).parse();
    Eigen::VectorXd f = solver->solve(g, H);
    std::string img = gen_image(f);
  };
};
