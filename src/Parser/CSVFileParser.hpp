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
    std::vector<std::pair<size_t, std::string_view>> indexed_lines = get_indexed_lines(lines);
    parallel_parse_lines(m, indexed_lines);

    return m;
  }
};