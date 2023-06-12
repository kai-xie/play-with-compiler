#pragma once

#include "BlockScope.h"
#include "PlayObject.h"
#include "Scope.h"
#include "ClassObject.h"
#include "FunctionObject.h"

namespace play {

class StackFrame {
 public:
  //该frame所对应的scope
  Scope* scope = nullptr;

  /**
   * 放parent scope所对应的frame的指针，就叫parentFrame吧，便于提高查找效率。
   * 规则：如果是同一级函数调用，跟上一级的parentFrame相同；
   * 如果是下一级的函数调用或For、If等block，parentFrame是自己；
   * 如果是一个闭包（如何判断？），那么要带一个存放在堆里的环境。
   */
  StackFrame* parentFrame{nullptr};

  //实际存放变量的地方
  PlayObject* object{nullptr};

  StackFrame(BlockScope* scope) {
    this->scope = scope;
    this->object = new PlayObject();
  }

  StackFrame(ClassObject* object) {
    this->scope = object->type;
    this->object = object;
  }

  /**
   * 为函数调用创建一个StackFrame
   * @param object
   */
  StackFrame(FunctionObject* object) {
    this->scope = object->function;
    this->object = object;
  }

  /**
   * 本栈桢里有没有包含某个变量的数据
   * @param variable
   * @return
   */
  bool contains(Variable* variable) {
    if (object && !object->fields.empty()) {
      return object->fields.count(variable) != 0;
    }
    return false;
  }

  std::string toString() {
    std::string rtn = scope->toString();
    if (parentFrame) {
      rtn += " -> " + parentFrame->toString();
    }
    return rtn;
  }

  friend std::ostream& operator<<(std::ostream& os, const StackFrame& sf) {
    return os << sf.toString();
  }
};

}  // namespace play
