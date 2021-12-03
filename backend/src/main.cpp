#include "Server.hpp"

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
  oatpp::base::Environment::init();

  Server::run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  OATPP_LOGI("Main",
             "\nEnvironment:\n objectsCount = %d\n objectsCreated = %d\n\n",
             oatpp::base::Environment::getObjectsCount(),
             oatpp::base::Environment::getObjectsCreated());
  
  oatpp::base::Environment::destroy();
}
