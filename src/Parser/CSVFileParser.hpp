#pragma once

#include <fstream>
#include <algorithm>
#include <execution>
#include <string>
#include <string_view>
#include <filesystem>

#include <Eigen/Dense>

#include "IParser.hpp"

class CSVFileToMatrixParser: public IParser<Eigen::MatrixXd> {
private:
  const std::filesystem::path& file_path;
  std::ifstream file;
  char separator;

  [[nodiscard]]
  int get_columns() {
    std::string line;
    std::getline(file, line);
    size_t x = std::count(line.begin(), line.end(), separator) + 1;
    file.seekg(0, std::ios::beg);
    return x;
  }

  [[nodiscard]]
  int get_rows() {
    std::istreambuf_iterator<char> it_file_start(file);
    std::istreambuf_iterator<char> it_file_end;
    size_t y = std::count(it_file_start, it_file_end, '\n');
    file.seekg(0, std::ios::beg);
    return y;
  }

  [[nodiscard]]
  std::string get_file_content() {
    std::string str(static_cast<size_t>(std::filesystem::file_size(file_path)), '\0');

    file.read(str.data(), str.size());

    return str;
  }

  [[nodiscard]]
  std::vector<std::string_view> split_string(std::string_view str, char delim) {
    std::vector<std::string_view> output;

    const auto last = str.end();
    for (auto first = str.begin(), second = str.begin(); second != last && first != last; first = std::next(second)) {
      second = std::find(first, last, delim);

      output.emplace_back(&*first, std::distance(first, second));

      if (second == last)
        break;
    }

    return output;
  }

  void parse_line(Eigen::MatrixXd& m, std::string_view line, size_t line_number) {
    std::vector<std::string_view> columns = split_string(line, separator);
    for (size_t j = 0; j < columns.size(); ++j) {
      m(line_number, j) = std::stod(std::string(columns[j]));
    }
  }

public:
  CSVFileToMatrixParser(const std::filesystem::path& file_path, char separator = ',')
  : file_path(file_path),
    file(file_path),
    separator(separator) {}

  ~CSVFileToMatrixParser() {
    file.close();
  }

  [[nodiscard]]
  Eigen::MatrixXd parse() {
    Eigen::MatrixXd m(get_rows(), get_columns());
    std::string file_content = get_file_content();
    std::vector<std::string_view> lines = split_string(file_content, '\n');
    std::vector<int> v(lines.size());
    std::iota(std::begin(v), std::end(v), 0);
    std::for_each(
      std::execution::par_unseq,
      std::begin(v),
      std::end(v),
      [&](int i) {
        parse_line(m, lines[i], i);
      }
    );

    return m;
  }
};