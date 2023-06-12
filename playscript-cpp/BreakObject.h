#pragma once

#include <string>

/**
 * 用于代表一个Break语句的对象
 */
namespace play {

class BreakObject {
 public:
  static BreakObject* instance() {
    static BreakObject inst = new BreakObject();
    return inst;
  }

  virtual std::string toString() const { return "Break"; }

 private:
  BreakObject() {}
};
}  // namespace play
