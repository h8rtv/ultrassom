#include "CGNESolver.hpp"

REGISTER_ALGORITHM_IMPL(CGNE, CGNESolver);

std::pair<Eigen::VectorXd, uint> CGNESolver::solve(const Eigen::VectorXd& g) {
  uint i;
  const Eigen::MatrixXd& H = modelMatrix.H;
  const Eigen::MatrixXd& Ht = modelMatrix.Ht;
  Eigen::VectorXd f = Eigen::VectorXd::Zero(H.cols());
  Eigen::VectorXd r = g - H * f;
  Eigen::VectorXd p = Ht * r;
  double r_old_norm = r.norm();
  for (i = 0; i < config.maxIterations; i++) {
    double alpha_num = r.transpose() * r;
    double alpha_den = p.transpose() * p;
    double alpha = alpha_num / alpha_den;
    r = r - alpha * H * p;
    double error = std::abs(r.norm() - r_old_norm);
    if (error < config.maxError) break;
    f = f + alpha * p;
    double beta_num = r.transpose() * r;
    const double& beta_den = alpha_num;
    double beta = beta_num / beta_den;
    p = Ht * r + beta * p;
    r_old_norm = r.norm();
  }

  return { f, i + 1 };
}