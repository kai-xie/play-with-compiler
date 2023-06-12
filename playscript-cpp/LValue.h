#pragma once

#include <string>

#include "PlayObject.h"
#include "Variable.h"

namespace play {

/**
 * 对栈中的值的引用
 */
class LValue {
 public:
  virtual antlrcpp::Any getValue() = 0;

  virtual void setValue(antlrcpp::Any value) = 0;

  virtual Variable* getVariable() = 0;

  virtual PlayObject* getValueContainer() = 0;

  virtual std::string toString() const = 0;
  // public StackFrame getFrame();
};

}  // namespace play
