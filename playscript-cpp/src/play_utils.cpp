
#include "play_utils.h"

#include <iostream>

namespace play {

absl::optional<std::string> ReadFile(const std::string& fileName) {
  std::ifstream ifs(fileName.c_str());
  if (!ifs.good()) {
    return absl::nullopt;
  }

  std::ifstream::pos_type fileSize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  std::vector<char> bytes(fileSize);
  ifs.read(bytes.data(), fileSize);

  return std::string(bytes.data(), fileSize);
}

void PrintLn(const std::string& content) {
  std::cout << content << std::endl;
}

}  // namespace play
