#pragma once

#include "ISolver.hpp"

class CGNRSolver : public ISolver {
public:
  Eigen::VectorXd solve(const Eigen::VectorXd& g, const Eigen::MatrixXd& H) {
    Eigen::VectorXd f = Eigen::VectorXd::Zero(H.cols());
    Eigen::VectorXd r = g - H * f;
    Eigen::VectorXd z = H.transpose() * r;
    Eigen::VectorXd p = z;
    double r_old_norm = r.norm();
    for (int i = 0; i < g.size(); i++) {
      auto w = H * p;
      double z_norm = std::pow(z.norm(), 2);
      double alpha =  z_norm / std::pow(w.norm(), 2);
      r = r - alpha * w;
      double error = std::abs(r.norm() - r_old_norm);
      if (error < 1e-4) break;
      f = f + alpha * p;
      z = H.transpose() * r;
      double beta = std::pow(z.norm(), 2) / z_norm;
      p = z + beta * p;
      r_old_norm = r.norm();
    }

    return f;
  }
};
