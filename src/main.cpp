#include <Eigen/Core>

#include "Server/Run.hpp"

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
  Eigen::initParallel();
  oatpp::base::Environment::init();

  run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  // std::cout << "Parsing g:" << std::endl;
  // auto finished1 = time_it();
  // Eigen::VectorXd g = CSVFileToMatrixParser("data/G-2.csv").parse();
  // finished1();
  // std::cout << "Done!" << std::endl;

  // std::cout << "Parsing H:" << std::endl;
  // auto finished8 = time_it();
  // Eigen::MatrixXd H = CSVFileToMatrixParser("data/H-1.csv").parse();
  // finished8();
  // std::cout << "Done!" << std::endl;

  // std::cout << "Computing signal gain:" << std::endl;
  // auto finished2 = time_it();
  // signal_gain(g);
  // finished2();
  // std::cout << "Done!" << std::endl;

  // std::cout << "Computing CGNR:" << std::endl;
  // auto finished5 = time_it();
  // CGNRSolver solver;
  // Eigen::VectorXd f = solver.solve(g, H);
  // finished5();
  // std::cout << "Done!" << std::endl;

  // std::string img = gen_image(f);

  // std::cout << "Writing aprox g:" << std::endl;
  // auto finished6 = time_it();
  // auto aprox_g = H * f;
  // std::ofstream aprox_g_file("out/aprox_g.txt");
  // aprox_g_file << aprox_g << std::endl;
  // finished6();
  // aprox_g_file.close();
  // std::cout << "Done!" << std::endl;

  // std::cout << "Writing output:" << std::endl;
  // auto finished7 = time_it();
  // std::ofstream image_file("out/image.txt");
  // image_file << f << std::endl;
  // finished7();
  // image_file.close();
  // std::cout << "Done!" << std::endl;
}
