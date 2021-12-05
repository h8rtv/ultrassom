#include "Server.hpp"

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
