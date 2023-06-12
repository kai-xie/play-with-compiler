#pragma once

#include "FunctionObject.h"
#include "LValue.h"

namespace play {

///////////////////////////////////////////////
//自己实现的左值对象。

class MyLValue : public LValue {
 public:
  PlayObject* valueContainer;
  Variable* variable;

  MyLValue(PlayObject* vcon, Variable* var)
      : valueContainer(vcon), variable(var) {}

  virtual antlrcpp::Any getValue() override {
    //对于this或super关键字，直接返回这个对象，应该是ClassObject
    if (antlrcpp::is<This*>(variable) || antlrcpp::is<Super*>(variable)) {
      return valueContainer;
    }

    return valueContainer->getValue(variable);
  }

  virtual void setValue(antlrcpp::Any value) override {
    valueContainer->setValue(variable, value);

    //如果variable是函数型变量，那改变functionObject.receiver
    if (FunctionObject* funcObj = value.as<FunctionObject*>()) {
      funcObj->receiver = variable;
    }
  }

  virtual Variable* getVariable() override { return variable; }

  virtual std::string toString() const override {
    return "LValue of " + variable->getName() + " : " +
           "antlrcpp::Any" /* getValue()->toString() */;
  }

  virtual PlayObject* getValueContainer() override { return valueContainer; }
};

}  // namespace play
