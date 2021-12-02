#include "CGNRSolver.hpp"

bool CGNRSolver::registered = AlgorithmFactory::register_algorithm<CGNRSolver>(
  "CGNR", [](const ModelMatrix& matrix, Settings::Config config) {
    return std::unique_ptr<CGNRSolver>(new CGNRSolver{matrix, config});
  }
);

Eigen::VectorXd CGNRSolver::solve(const Eigen::VectorXd& g) {
  const Eigen::MatrixXd& H = modelMatrix.H;
  const Eigen::MatrixXd& Ht = modelMatrix.Ht;
  Eigen::VectorXd f = Eigen::VectorXd::Zero(H.cols());
  Eigen::VectorXd r = g - H * f;
  Eigen::VectorXd z = Ht * r;
  Eigen::VectorXd p = z;
  double r_old_norm = r.norm();
  for (int i = 0; i < config.maxIterations; i++) {
    auto w = H * p;
    double z_norm = std::pow(z.norm(), 2);
    double alpha =  z_norm / std::pow(w.norm(), 2);
    r = r - alpha * w;
    double error = std::abs(r.norm() - r_old_norm);
    if (error < config.maxError) break;
    f = f + alpha * p;
    z = Ht * r;
    double beta = std::pow(z.norm(), 2) / z_norm;
    p = z + beta * p;
    r_old_norm = r.norm();
  }

  return f;
}