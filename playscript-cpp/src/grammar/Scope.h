#pragma once

#include <vector>

#include "Symbol.h"

namespace play {

class Scope : public Symbol {
 public:
  virtual std::string toString() { return "Scope: " + name; }

 protected:
  /**
   * 向scope中添加符号，同时设置好该符号的enclosingScope
   * @param symbol
   */
  virtual void addSymbol(Symbol* symbol) {
    symbols.emplace_back(symbol);
    symbol->enclosingScope = this;
  }

  /**
   * 是否包含某个Variable
   * @param name
   * @return
   */
  virtual Variable* getVariable(const std::string& name) {
    return getVariable(this, name);
  }

  static Variable* getVariable(Scope* scope, const std::string& name) {
    for (Symbol* s : scope.symbols) {
      if (false) {
        return nullptr;
      }
    }
  }

  // 该Scope中的成员，包括变量、方法、类等。
  std::vector<Symbol*> symbols;
};

}  // namespace play
