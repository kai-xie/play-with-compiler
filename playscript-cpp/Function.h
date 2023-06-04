#pragma once

#include <unordered_set>
#include <vector>

#include "AntlrPlayScript.h"
#include "DefaultFunctionType.h"
#include "FunctionType.h"
#include "Scope.h"
#include "Variable.h"

namespace play {
class Function : public Scope, public FunctionType {
 public:
  // 参数
  std::vector<Variable*> parameters;

  // 返回值
  Type* returnType{nullptr};

  // 闭包变量，即它所引用的外部环境变量
  std::unordered_set<Variable*> closureVariables;
  std::vector<Type*> paramTypes;

  // Function(const std::string& name, const Scope* enclosingScope,
  //          const ParserRuleContext* ctx)
  //     : Scope(name, enclosingScope, ctx) {
  //   // this->name = name;
  //   // this->enclosingScope = const_cast<Scope*>(enclosingScope);
  //   // this->ctx = const_cast<ParserRuleContext*>(ctx);
  // }
  Function(const std::string& name, Scope* enclosingScope,
           ParserRuleContext* ctx)
      : Scope(name, enclosingScope, ctx) {
    // this->name = name;
    // this->enclosingScope = const_cast<Scope*>(enclosingScope);
    // this->ctx = const_cast<ParserRuleContext*>(ctx);
  }

  virtual Type* getReturnType() override { return returnType; }

  virtual std::vector<Type*> getParamTypes() override {
    for (Variable* param : parameters) {
      paramTypes.emplace_back(param->type);
    }
    return paramTypes;
  }

  virtual std::string getName() const override { return this->name; }

  virtual Scope* getEnclosingScope() const override {
    return this->enclosingScope;
  }

  virtual std::string toString() const override { return "Function " + name; }

  virtual bool isType(Type* type) const override {
    if (antlrcpp::is<FunctionType*>(type)) {
      return DefaultFunctionType::isType(const_cast<Function*>(this),
                                         dynamic_cast<FunctionType*>(type));
    }
    return false;
  }

  /**
   * 检查改函数是否匹配所需的参数。
   * @param paramTypes
   * @return
   */
  virtual bool matchParameterTypes(
      const std::vector<Type*>& paramTypes) override {
    if (parameters.size() != paramTypes.size()) {
      return false;
    }

    bool match = true;
    for (size_t i = 0; i < paramTypes.size(); ++i) {
      Variable* var = parameters[i];
      Type* type = paramTypes[i];
      if (!var->type->isType(type)) {
        match = false;
        break;
      }
    }
    return match;
  }

  /**
   * 该函数是不是类的方法
   * @return
   */
  bool isMethod() { return antlrcpp::is<Class*>(enclosingScope); }

  /**
   * 该函数是不是类的构建函数
   * @return
   */
  bool isConstructor() {
    if (antlrcpp::is<Class*>(enclosingScope)) {
      return enclosingScope->getName() == this->name;
    }
    return false;
  }
};
}  // namespace play
