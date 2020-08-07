#pragma once

#include <vector>

#include "grammar/Symbol.h"

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
    symbol.enclosingScope = this;
  }

virtual Variable* getVariable(const std::string& name) {
  return getVariable(this, name);
}

static Variable* getVariable(Scope* scope, const std::string& name) {
  for (Symbol* s : scope.symbols) {
    if ()
  }
}


  // 该Scope中的成员，包括变量、方法、类等。
  std::vector<Symbol*> symbols;
};

}  // namespace play
