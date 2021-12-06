#include "CGNRSolver.hpp"

REGISTER_ALGORITHM_IMPL(CGNR, CGNRSolver);

std::pair<Eigen::VectorXd, uint> CGNRSolver::solve(const Eigen::VectorXd& g) {
  uint i;
  const Eigen::MatrixXd& H = modelMatrix.H;
  const Eigen::MatrixXd& Ht = modelMatrix.Ht;
  Eigen::VectorXd f = Eigen::VectorXd::Zero(H.cols());
  Eigen::VectorXd r = g - H * f;
  Eigen::VectorXd z = Ht * r;
  Eigen::VectorXd p = z;
  double r_old_norm = r.norm();
  for (i = 0; i < config.maxIterations; i++) {
    auto w = H * p;
    double z_norm = std::pow(z.norm(), 2);
    double alpha =  z_norm / std::pow(w.norm(), 2);
    f = f + alpha * p;
    r = r - alpha * w;
    double error = std::abs(r.norm() - r_old_norm);
    if (error < config.maxError) break;
    z = Ht * r;
    double beta = std::pow(z.norm(), 2) / z_norm;
    p = z + beta * p;
    r_old_norm = r.norm();
  }

  if (i >= config.maxIterations) {
    i = config.maxIterations;
  }

  return { f, i + 1 };
}