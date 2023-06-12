#pragma once

#include <ostream>

#include "ClassObject.h"

namespace play {
/**
 * 用来代表null值的对象。
 * 采用单例模式。用instance()方法来获得一个对象实例。
 */
class NullObject : public ClassObject {
 public:
  //获取唯一的实例。
  static NullObject* instance() {
    static NullObject* instance = new NullObject();
    return instance;
  }

  virtual std::string toString() const override { return "Null"; }

  //在打印时输出Null。
  friend std::ostream& operator<<(std::ostream& os, const NullObject& no) {
    return os << no.toString();
  }

 private:
  NullObject() {}
};
}  // namespace play
