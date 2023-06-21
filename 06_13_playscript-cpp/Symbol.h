#pragma once

#include <string>

#include "AntlrPlayScript.h"

namespace play {
class Scope;

class Symbol {
 protected:
  Symbol(const std::string& name, Scope* enScope, ParserRuleContext* context)
      : name(name), enclosingScope(enScope), ctx(context) {}

 public:
  // 符号的名称
  std::string name;

  // 所属作用域
  Scope* enclosingScope{nullptr};

  // 可见性，比如public还是private
  int visibility = 0;

  // Symbol关联的AST节点
  ParserRuleContext* ctx{nullptr};

  std::string getName() const { return name; }

  Scope* getEnclosingScope() const { return enclosingScope; }

  virtual std::string toString() const = 0;
};

}  // namespace play
