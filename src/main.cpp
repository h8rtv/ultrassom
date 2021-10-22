#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>

template<class ParseResult>
class Parser {
public:
  virtual ParseResult parse() = 0;
};

class CSVToMatrixParser : public Parser<Eigen::MatrixXd> {
private:
  std::istream& file;
  char separator;

  int getColumns() {
    std::string line;
    std::getline(file, line);
    size_t x = std::count(line.begin(), line.end(), separator) + 1;
    file.seekg(0, std::ios::beg);
    return x;
  }

  int getRows() {
    std::istreambuf_iterator<char> it_file_start(file);
    std::istreambuf_iterator<char> it_file_end;
    size_t y = std::count(it_file_start, it_file_end, '\n');
    file.seekg(0, std::ios::beg);
    return y;
  }

public:
  CSVToMatrixParser(std::istream& file, char separator = ','): file(file), separator(separator) {}

  Eigen::MatrixXd parse() {
    Eigen::MatrixXd m(getRows(), getColumns());
    int row = 0;
    for (std::string line; std::getline(file, line); row++) {
      std::string number;
      int col = 0;
      for (char& c: line) {
        if (c == separator)  {
          m(row, col++) = std::stod(number);
          number = "";
        } else {
          number += c;
        }
      }
      m(row, col++) = std::stod(number);
    }

    return m;
  }
};

class CSVFileToMatrixParser: public Parser<Eigen::MatrixXd> {
private:
  const std::string& filename;
  std::ifstream file;

public:
  CSVFileToMatrixParser(const std::string& filename): filename(filename), file(filename) {};
  Eigen::MatrixXd parse() {
    CSVToMatrixParser parser(file);
    Eigen::MatrixXd m = parser.parse();
    return m;
  }
};

void test() {
  Eigen::MatrixXd a = CSVFileToMatrixParser("data/a.csv").parse();
  Eigen::MatrixXd M = CSVFileToMatrixParser("data/M.csv").parse();
  Eigen::MatrixXd N = CSVFileToMatrixParser("data/N.csv").parse();

  std::cout << "a:" << std::endl << a << std::endl;
  std::cout << "M:" << std::endl << M << std::endl;
  std::cout << "N:" << std::endl << N << std::endl;

  Eigen::MatrixXd MN = M * N;
  Eigen::MatrixXd aM = a * M;
  Eigen::MatrixXd Ma = M * a.transpose();

  std::cout << "M * N:"    << std::endl << MN << std::endl;
  std::cout << "a * M:"    << std::endl << aM << std::endl;
  std::cout << "M * a(T):" << std::endl << Ma << std::endl;

  Eigen::MatrixXd MN_answer = CSVFileToMatrixParser("data/MN.csv").parse();
  Eigen::MatrixXd aM_answer = CSVFileToMatrixParser("data/aM.csv").parse();

  assert(MN.isApprox(MN_answer, 0.0001));
  assert(aM.isApprox(aM_answer, 0.0001));
}

Eigen::VectorXd cgnr(const Eigen::VectorXd& g, const Eigen::MatrixXd& H) {
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

std::function<void()> time_it() {
  auto start = std::chrono::system_clock::now();
  return [start]() {
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elasped = end - start;
    std::cout << elasped.count() << " seconds" << std::endl;
  };
}

double reduction_factor(const Eigen::MatrixXd& H) {
  return (H.transpose() * H).norm();
}

double regularization_coefficient(const Eigen::VectorXd& g, const Eigen::MatrixXd& H) {
  return (H.transpose() * g).cwiseAbs().maxCoeff();
}

int main() {
  std::cout << "Parsing g:" << std::endl;
  auto finished1 = time_it();
  Eigen::VectorXd g = CSVFileToMatrixParser("data/g-1.txt").parse();
  finished1();
  std::cout << "Done!" << std::endl;

  std::cout << "Parsing H:" << std::endl;
  auto finished2 = time_it();
  Eigen::MatrixXd H = CSVFileToMatrixParser("data/H-1.txt").parse();
  finished2();
  std::cout << "Done!" << std::endl;

  std::cout << "Computing CGNR:" << std::endl;
  auto finished5 = time_it();
  Eigen::VectorXd f = cgnr(g, H);
  finished5();
  std::cout << "Done!" << std::endl;

  std::cout << f.minCoeff() << " " << f.maxCoeff() << std::endl;

  std::cout << "Writing aprox g:" << std::endl;
  auto finished6 = time_it();
  auto aprox_g = H * f;
  std::ofstream aprox_g_file("out/aprox_g.txt");
  aprox_g_file << aprox_g << std::endl;
  finished6();
  std::cout << "Done!" << std::endl;

  std::cout << "Writing output:" << std::endl;
  auto finished7 = time_it();
  std::ofstream image_file("out/image.txt");
  image_file << f << std::endl;
  finished7();
  std::cout << "Done!" << std::endl;
}
