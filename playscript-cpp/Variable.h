#pragma once

#include <iostream>
#include <string>

#include "AntlrPlayScript.h"
#include "Symbol.h"
#include "Type.h"
// #include "Class.h"

namespace play {
class Class;

class Variable : public Symbol {
 public:
  // 变量类型
  Type* type{nullptr};

  //// 作为parameter的变量的属性
  // 缺省值
  void* defaultValue{nullptr};

  // 是否允许多次重复，这是一个创新的参数机制
  int multiplicity = 1;

  // Variable(const std::string& name, const Scope* enclosingScope,
  //          const ParserRuleContext* ctx)
  //     : Symbol(name, enclosingScope, ctx) {}
  Variable(const std::string& name, Scope* enclosingScope,
           ParserRuleContext* ctx)
      : Symbol(name, enclosingScope, ctx) {}

  /**
   * 是不是类的属性
   * @return
   */
  bool isClassMember() { return antlrcpp::is<Class*>(enclosingScope); }

  virtual std::string toString() const override {
    return std::string("Variable ") + name + " -> " + type->toString();
  }

  friend std::ostream& operator<<(std::ostream& os, const Variable& var) {
    return os << var.toString();
  }
};

}  // namespace play
