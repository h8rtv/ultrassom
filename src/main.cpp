#include "Server.hpp"
#include "AlgebraState.hpp"

void signal_gain(Eigen::VectorXd& g) {
  uint N = 64;
  uint S = 794;
  for (uint c = 0; c < N; c++) {
    for (uint l = 0; l < S; l++) {
      uint y = 100 + 1 / 20 * (l + 1) * sqrt(l + 1);
      uint index = l + S * c;
      g(index) = g(index) * y;
    }
  }
}

int main() {
  // Init Algebra State (Model Matrix)
  static_cast<void>(AlgebraState::instance());

  oatpp::base::Environment::init();

  Server::run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
}
