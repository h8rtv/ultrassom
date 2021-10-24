#pragma once

#include <iostream>
#include <Eigen/Dense>

#include "IParser.hpp"

class CSVToMatrixParser : public IParser<Eigen::MatrixXd> {
private:
  std::istream& input_stream;
  char separator;

  int get_columns() {
    std::string line;
    std::getline(input_stream, line);
    size_t x = std::count(line.begin(), line.end(), separator) + 1;
    input_stream.seekg(0, std::ios::beg);
    return x;
  }

  int get_rows() {
    std::istreambuf_iterator<char> it_file_start(input_stream);
    std::istreambuf_iterator<char> it_file_end;
    size_t y = std::count(it_file_start, it_file_end, '\n');
    input_stream.seekg(0, std::ios::beg);
    return y;
  }

public:
  CSVToMatrixParser(std::istream& input_stream, char separator = ','): input_stream(input_stream), separator(separator) {}

  Eigen::MatrixXd parse() {
    Eigen::MatrixXd m(get_rows(), get_columns());
    int row = 0;
    for (std::string line; std::getline(input_stream, line); row++) {
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
