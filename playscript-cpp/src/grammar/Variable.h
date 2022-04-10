#pragma once

#include <type_traits>

#include "Symbol.h"
#include "antlr4-runtime.h"

namespace play {

class Varible : public Symbol {
 public:
  bool IsClassMember() { return std::is_base_of<Class, decltype(*enclosingScope)>::value; }

  std::string toString() {
    return "Variable " + name + " -> " + type;
  }
};

}  // namespace play
