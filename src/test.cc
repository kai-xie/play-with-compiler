#include <iostream>
#include <memory>

#include "glog/logging.h"
#include "absl/memory/memory.h"

// std::ostream & operator<<(std::ostream &s, std::nullptr_t) {
//     return s << static_cast<void *>(nullptr);
// }

std::ostream& operator<<(std::ostream& s, std::nullptr_t) {
  return s << "nullptr";
}

int main() {
  // std::nullptr_t n1;
  // std::cout << n1 << std::endl;

  std::unique_ptr<std::string> pstr;
  // pstr = std::make_unique<std::string>("hello");
  // CHECK(pstr.get()) << "pstr is NULL";

  if (pstr.get() == nullptr) {
    VLOG(0) << "pstr.get() is nullptr";
  }
  if (pstr == nullptr) {
    VLOG(0) << "pstr is nullptr";
  }

  CHECK(!pstr) << "pstr is NULL";
  // CHECK_NE(pstr, nullptr) << "pstr is not nullptr";

  // pstr = std::move(new std::string("hello"));
  // pstr = std::move(new std::string("hello"));
  // pstr = absl::WrapUnique(new std::string("hello"));
  // pstr.reset(new std::string("hello"));
  pstr = std::unique_ptr<std::string>(new std::string("hello"));
  VLOG(0) << "pstr : " << *pstr;

  return 0;
}
