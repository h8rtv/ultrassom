#pragma once

#include <Eigen/Dense>

struct ModelMatrix {
  Eigen::MatrixXd H;
  Eigen::MatrixXd Ht;
};