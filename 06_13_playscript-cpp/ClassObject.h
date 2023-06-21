#pragma once

#include <string>

#include "Class.h"
#include "PlayObject.h"

namespace play {
class ClassObject : public PlayObject {
 public:
  Class* type = nullptr;

  virtual std::string toString() const override {
    return "ClassObject: " + type->toString();
  }

};
}  // namespace play
