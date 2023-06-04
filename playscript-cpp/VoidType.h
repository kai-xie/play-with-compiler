#pragma once

#include <string>

#include "Type.h"

namespace play {
class Scope;

class VoidType : public Type {
 public:
  std::string getName() override { return "void"; }

  Scope* getEnclosingScope() override { return nullptr; }

  bool isType(Type* type) override { return this == type; }

  VoidType instance() { return *voidType; }

  std::string toString() { return "void"; }

  friend std::ostream& operator<<(std::ostream& os, const VoidType& voidType) {
    return os << voidType.toString();
  }

 private:
  static VoidType* voidType = new VoidType();

  VoidType() == default;
};
}  // namespace play
