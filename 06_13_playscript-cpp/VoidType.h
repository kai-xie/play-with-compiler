#pragma once

#include <string>

#include "Type.h"

namespace play {
class Scope;

class VoidType : public Type {
 public:
  virtual std::string getName() const override { return "void"; }

  virtual Scope* getEnclosingScope() const override { return nullptr; }

  virtual bool isType(Type* type) const override { return this == type; }

  virtual std::string toString() const override { return "void"; }

  friend std::ostream& operator<<(std::ostream& os, const VoidType& voidType) {
    return os << voidType.toString();
  }

  static VoidType* instance() {
    static VoidType* voidType = new VoidType();
    return voidType;
  }

 private:
  VoidType() = default;
};
}  // namespace play
