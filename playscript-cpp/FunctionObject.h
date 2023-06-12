#pragma once

#include "Function.h"
#include "PlayObject.h"
#include "Variable.h"

namespace play {

/**
 * 存放一个函数运行时的本地变量的值，包括参数的值。
 */
class FunctionObject : public PlayObject {
 public:
  //类型
  Function* function{nullptr};

  /**
   * 接收者所在的scope。缺省是function的enclosingScope，也就是词法的Scope。
   * 当赋值给一个函数型变量的时候，要修改receiverEnclosingScope等于这个变量的enclosingScope。
   */
  Variable* receiver{nullptr};

  FunctionObject(Function* func) : function(func) {}

  void setFunction(Function* func) { this->function = func; }

  virtual std::string toString() const override {
    return "FunctionObject: " + function->toString();
  }

};
}  // namespace play
