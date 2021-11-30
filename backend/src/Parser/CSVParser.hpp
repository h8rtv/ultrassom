#pragma once

#include <algorithm>
#include <execution>
#include <string_view>

#include <Eigen/Dense>

#include "IParser.hpp"

class CSVParser: public IParser<Eigen::MatrixXd> {
private:
  char separator;
  std::string_view file_content;

  [[nodiscard]]
  virtual size_t get_column_count(std::string_view line) {
    size_t x = std::count(line.begin(), line.end(), separator) + 1;

    return x;
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

  [[nodiscard]]
  std::vector<std::pair<size_t, std::string_view>> get_indexed_lines(const std::vector<std::string_view>& lines) {
    std::vector<std::pair<size_t, std::string_view>> indexed_lines;
    indexed_lines.reserve(lines.size());
    std::transform(
      lines.begin(),
      lines.end(),
      std::back_inserter(indexed_lines),
      [n = 0] (std::string_view line) mutable {
        return std::make_pair(n++, line);
      }
    );
    return indexed_lines;
  }

  inline void parse_line(Eigen::MatrixXd& m, std::string_view line, size_t line_number) {
    std::vector<std::string_view> columns = split_string(line, separator);
    for (size_t j = 0; j < columns.size(); ++j) {
      m(line_number, j) = std::stod(std::string(columns[j]));
    }
  }

  void parallel_parse_lines(Eigen::MatrixXd& m, std::vector<std::pair<size_t, std::string_view>> indexed_lines) {
    std::for_each(
      std::execution::par_unseq,
      std::begin(indexed_lines),
      std::end(indexed_lines),
      [&](std::pair<size_t, std::string_view> pair) {
        auto [i, line] = pair;
        parse_line(m, line, i);
      }
    );
  }

public:
  CSVParser(std::string_view file_content, char separator = ',') : file_content(file_content), separator(separator) {}
  ~CSVParser() { }

  [[nodiscard]]
  Eigen::MatrixXd parse() {
    std::vector<std::string_view> lines = split_string(file_content, '\n');
    size_t row = lines.size();
    size_t col = get_column_count(lines[0]);
    Eigen::MatrixXd m(row, col);
    std::vector<std::pair<size_t, std::string_view>> indexed_lines = get_indexed_lines(lines);
    parallel_parse_lines(m, indexed_lines);

    return m;
  }
};