#pragma once

#include <string>

#include "Class.h"
#include "Function.h"

namespace play {
// class Class;

class DefaultConstructor : public Function {
 public:
  DefaultConstructor(const std::string& name, Class* theClass)
      : Function(name, (Scope*)(theClass), nullptr) {}

  Class* belongingClass() { return dynamic_cast<Class*>(enclosingScope); }

  virtual std::string toString() const override {
    return "DefaultConstructor: " + this->name;
  }
};
}  // namespace play
