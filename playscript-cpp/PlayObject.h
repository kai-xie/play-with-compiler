#pragma once

#include <unordered_map>

#include "AntlrPlayScript.h"

namespace play {

class Variable;
/**
 * PlayScript的对象
 */
class PlayObject {
  //成员变量
 public:
  std::unordered_map<Variable*, antlrcpp::Any> fields;

  virtual antlrcpp::Any getValue(Variable* variable);

  virtual void setValue(Variable* variable, antlrcpp::Any value) {
    fields[variable] = value;
  }

  virtual std::string toString() const {
    return "PlayObject";
  };

};

}  // namespace play
