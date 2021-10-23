#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>

#include "Parser.hpp"

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