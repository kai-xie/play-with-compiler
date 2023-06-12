#pragma once

#include "AntlrPlayScript.h"

namespace play {
/**
 * 代表Return语句的返回值
 */
class ReturnObject {
 public:
  antlrcpp::Any returnValue;  // the real return value.
  ReturnObject(antlrcpp::Any value) { this->returnValue = value; }

  virtual std::string toString() { return "ReturnObject"; }
};

}  // namespace play
