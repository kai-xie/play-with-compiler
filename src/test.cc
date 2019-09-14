#include <iostream>
#include <memory>

#include "glog/logging.h"

// std::ostream & operator<<(std::ostream &s, std::nullptr_t) {
//     return s << static_cast<void *>(nullptr);
// }

std::ostream& operator<<(std::ostream& s, std::nullptr_t) {
  return s << "nullptr";
}

int main() {
  std::nullptr_t n1;
  std::cout << n1 << std::endl;

  std::unique_ptr<std::string> pstr;
  // pstr = std::make_unique<std::string>("hello");
  // CHECK(pstr.get()) << "pstr is NULL";

  if (pstr.get() == nullptr) {
    VLOG(0) << "pstr.get() is nullptr";
  }

  if (pstr == nullptr) {
    VLOG(0) << "pstr is nullptr";
  } else if (pstr.get() == nullptr) {
    VLOG(0) << "pstr.get() is nullptr";
  }

  CHECK(pstr) << "pstr is NULL";

  return 0;
}
