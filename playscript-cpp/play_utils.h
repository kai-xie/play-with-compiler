#pragma once

#include <string>

#include "absl/types/optional.h"

namespace play {
// namespace internal {
/**
 * @brief read contents from file into a string.
 *
 * @param fileName
 * @return absl::optional<std::string>
 */
absl::optional<std::string> ReadFile(const std::string& fileName);

void PrintLn(const std::string& content);

template <typename Ptr>
void FreePtr(Ptr ptr) {
  if (ptr != nullptr) {
    delete ptr;
  }
}

template <typename Ptr, typename... RestPtrs>
void FreePtr(Ptr head, RestPtrs... rest) {
  if (head != nullptr) {
    delete head;
  }
  FreePtr(rest...);
}

// }  // namespace internal
}  // namespace play
