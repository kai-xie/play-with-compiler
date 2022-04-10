#pragma once

#include <string>

#include "antlr4-runtime.h"
#include "play_utils.h"

namespace play {

class Scope;

// An abstract class for symbols.
class Symbol {
 public:
  Symbol() = default;
  virtual ~Symbol() {
    FreePtr(enclosingScope, ctx);
  }

  std::string getName() { return name; }

  Scope* getEnclosingScope() { return enclosingScope; }

  // friend class Scope;
//  protected:
  //符号的名称
  std::string name = "";

  //所属作用域
  Scope* enclosingScope = nullptr;

  //可见性，比如public还是private
  int visibility = 0;

  // Symbol关联的AST节点
  antlr4::ParserRuleContext* ctx = nullptr;
}

}  // namespace play
