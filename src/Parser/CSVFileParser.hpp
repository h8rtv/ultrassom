#pragma once

#include <fstream>

#include "CSVParser.hpp"

class CSVFileToMatrixParser: public IParser<Eigen::MatrixXd> {
private:
  const std::string& filename;
  std::ifstream file;

public:
  CSVFileToMatrixParser(const std::string& filename): filename(filename), file(filename) {}
  ~CSVFileToMatrixParser() {
    file.close();
  }
  Eigen::MatrixXd parse() {
    CSVToMatrixParser parser(file);
    Eigen::MatrixXd m = parser.parse();
    return m;
  }
};