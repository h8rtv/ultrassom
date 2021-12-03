#pragma once

#include <fstream>
#include <algorithm>
#include <execution>
#include <string>
#include <string_view>
#include <filesystem>

#include <Eigen/Dense>

#include "CSVParser.hpp"

class CSVFileParser: public IParser<Eigen::MatrixXd> {
private:
  const std::filesystem::path& file_path;
  std::ifstream file;
  char separator;

  [[nodiscard]]
  std::string get_file_content() {
    std::string str(static_cast<size_t>(std::filesystem::file_size(file_path)), '\0');

    file.read(str.data(), str.size());

    return str;
  }

public:
  CSVFileParser(const std::filesystem::path& file_path, char separator = ',')
  : file_path(file_path),
    file(file_path),
    separator(separator) {}

  ~CSVFileParser() {
    file.close();
  }

  [[nodiscard]]
  Eigen::MatrixXd parse() {
    std::string file_content = get_file_content();
    CSVParser parser {file_content, separator};

    return parser.parse();
  }
};