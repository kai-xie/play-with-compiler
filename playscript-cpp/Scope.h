#pragma once

#include <vector>

// #include "AntlrPlayScript.h"
#include "Symbol.h"
// #include "Type.h"
// #include "Variable.h"

namespace play {

class Variable;
class Type;
class Function;
class Class;

class Scope : public Symbol {
 public:
  Scope(const std::string& name, Scope* enclosingScope, ParserRuleContext* ctx)
      : Symbol(name, enclosingScope, ctx) {
    // this->name = name;
    // this->enclosingScope = const_cast<Scope*>(enclosingScope);
    // this->ctx = const_cast<ParserRuleContext*>(ctx);
  }

  // 该Scope中的成员，包括变量、方法、类等。
  std::vector<Symbol*> symbols;

  /**
   * 向scope中添加符号，同时设置好该符号的enclosingScope
   * @param symbol
   */
  void addSymbol(Symbol* symbol) {
    symbols.emplace_back(symbol);
    symbol->enclosingScope = this;
  }

  /**
   * 是否包含某个Variable
   * @param name
   * @return
   */
  virtual Variable* getVariable(const std::string& name) {
    return Scope::getVariable(const_cast<Scope*>(this), name);
  }

  static Variable* getVariable(Scope* scope, const std::string& name);

  /**
   * 是否包含某个Function
   * @param name
   * @param paramTypes
   * 参数类型。不允许为空。该参数不允许为空。如果没有参数，需要传入一个0长度的列表。
   * @return
   */

  virtual Function* getFunction(const std::string& name,
                                const std::vector<Type*>& paramTypes) {
    return Scope::getFunction(const_cast<Scope*>(this), name, paramTypes);
  }

  /**
   * 是否包含某个Function。这是个静态方法，可以做为工具方法重用。避免类里要超找父类的情况。
   * @param scope
   * @param name
   * @param paramTypes
   * @return
   */
  static Function* getFunction(Scope* scope, const std::string& name,
                               const std::vector<Type*>& paramTypes);

  /**
   * 获取一个函数类型的变量，能匹配相应的参数类型
   * @param name
   * @param paramTypes
   * @return
   */
  virtual Variable* getFunctionVariable(const std::string& name,
                                        const std::vector<Type*>& paramTypes) {
    return Scope::getFunctionVariable(const_cast<Scope*>(this), name,
                                      paramTypes);
  }

  static Variable* getFunctionVariable(Scope* scope, const std::string& name,
                                       const std::vector<Type*>& paramTypes);

  /**
   * 是否包含某个Class
   * @param name
   * @return
   */
  virtual Class* getClass(const std::string& name) {
    return Scope::getClass(const_cast<Scope*>(this), name);
  }

  static Class* getClass(Scope* scope, const std::string& name);

  /**
   * 是否包含某个Symbol
   * @param symbol
   * @return
   */
  virtual bool containsSymbol(Symbol* symbol) {
    return std::any_of(symbols.cbegin(), symbols.cend(),
                       [&](Symbol* s) { return s == symbol; });
  }

  virtual std::string toString() const override { return "Scope: " + name; }

  friend std::ostream& operator<<(std::ostream& os, const Scope& s) {
    return os << s.toString();
  }
};

}  // namespace play
