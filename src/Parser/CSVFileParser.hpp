#pragma once

#include <fstream>
#include <charconv>
#include <string>
#include <string_view>
#include <filesystem>

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
    // TODO: tentar paralelizar isso aqui
    for (size_t i = 0; i < lines.size(); ++i) {
      std::vector<std::string_view> columns = split_string(lines[i], separator);
      for (size_t j = 0; j < columns.size(); ++j) {
        // LOL, lento pra krl no gcc 11 hahahahahah
        // https://www.reddit.com/r/cpp/comments/na7tut/why_is_stdfrom_charsfloat_slow/
        // TODO: testar no MSVC que aparentemente otimiza essa operação
        // https://youtu.be/4P_kbF0EbZM
        // auto sv = columns[j];
        // double value;
        // const auto last = sv.data() + sv.size();
        // const auto res = std::from_chars(sv.data(), last, value);
        // Por hora vou criar string temporária, é rápido	o suficiente
        m(i, j) = std::stod(std::string(columns[j]));
      }
    }

    return m;
  }
};