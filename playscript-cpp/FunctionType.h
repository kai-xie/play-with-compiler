#pragma once

#include <vector>

#include "Type.h"

namespace play {

/**
 * 函数类型
 */
class FunctionType : public Type {
 public:
  virtual Type* getReturnType() = 0;

  virtual std::vector<Type*> getParamTypes() = 0;

  virtual bool matchParameterTypes(const std::vector<Type*>& paramTypes) = 0;
};
}  // namespace play
