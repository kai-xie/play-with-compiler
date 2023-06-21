#include "play_utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace play {

absl::optional<std::string> ReadFile(const std::string& filename) {
  // std::ifstream ifs(filename.c_str());
  // if (!ifs.good()) {
  //   return absl::nullopt;
  // }

  // std::ifstream::pos_type fileSize = ifs.tellg();
  // ifs.seekg(0, std::ios::beg);

  // std::vector<char> bytes(fileSize);
  // ifs.read(bytes.data(), fileSize);

  // return std::string(bytes.data(), fileSize);

  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  file.close();

  return buffer.str();
}

void PrintLn(const std::string& content) { std::cout << content << std::endl; }

}  // namespace play
