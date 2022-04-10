#pragma once

#include <string>

#include "Scope.h"



namespace play {

// An interface class for Type
class Type {
 public:
  virtual std::string getName() = 0;

  virtual Scope getEnclosingScope() = 0;

  /**
   * 本类型是不是 is 目标类型。 也就是能否用来替换目标类型。
   * 以面向对象为例，子类 is 父类。子类可以出现在任何需要父类的地方。
   * @param type 目标类型
   * @return
   */
  bool isType(Type type) = 0;
}

}  // namespace play
