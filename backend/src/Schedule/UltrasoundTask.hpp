#pragma once

#include <string_view>
#include <Eigen/Dense>

#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageGeneration.hpp"

class UltrasoundTask {
private:
  u_int32_t user;
  std::string_view algorithm;
  std::string_view data;
  const std::shared_ptr<Eigen::MatrixXd> H;

public:
  UltrasoundTask(
    u_int32_t user,
    std::string_view algorithm,
    std::string_view data,
    const std::shared_ptr<Eigen::MatrixXd> H)
  : user(user),
    algorithm(algorithm),
    data(data),
    H(H)
  {};

  void operator()() const {
    auto solver = AlgorithmFactory::create(algorithm);
    Eigen::VectorXd g = CSVParser(data).parse();
    Eigen::VectorXd f = solver->solve(g, (*H));
    std::string img = gen_image(f);
  };
};
