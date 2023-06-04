#pragma once

#include <string>

// #include "AntlrPlayScript.h"
#include "Scope.h"

namespace play {
class BlockScope : public Scope {
 private:
  static int index;

 public:
  BlockScope() : Scope("block" + std::to_string(index++), nullptr, nullptr) {}

  BlockScope(Scope* enclosingScope, ParserRuleContext* ctx)
      : Scope("block" + std::to_string(index++), enclosingScope, ctx) {
    // BlockScope();
    // // this->name = "block" + std::to_string(index++);
    // this->enclosingScope = enclosingScope;
    // this->ctx = ctx;
  }

 protected:
  BlockScope(const std::string& name, Scope* enclosingScope,
             ParserRuleContext* ctx)
      : Scope(name, enclosingScope, ctx) {}

  virtual std::string toString() const override {
    return "Block " + this->name;
  }

  friend std::ostream& operator<<(std::ostream& os, const BlockScope& bs) {
    return os << bs.toString();
  }
};

}  // namespace play
