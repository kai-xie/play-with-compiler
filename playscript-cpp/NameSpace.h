#pragma once

#include <string>
// #include <vector>
#include <unordered_set>

#include "AntlrPlayScript.h"
#include "BlockScope.h"

namespace play {

/**
 * 命名空间。
 */
class NameSpace : public BlockScope {
 private:
  const NameSpace* parent{nullptr};

  // std::vector<NameSpace*> subNameSpaces;
  std::unordered_set<NameSpace*> subNameSpaces;

  // std::string name;

 public:
  // NameSpace(const std::string& name, const Scope* enclosingScope,
  //           const ParserRuleContext* ctx)
  //     : BlockScope(name, enclosingScope, ctx) {
  //   // this->name = name;
  //   // this->enclosingScope = enclosingScope;
  //   // this->ctx = ctx;
  // }
  NameSpace(const std::string& name, Scope* enclosingScope,
            ParserRuleContext* ctx)
      : BlockScope(name, enclosingScope, ctx) {
    // this->name = name;
    // this->enclosingScope = enclosingScope;
    // this->ctx = ctx;
  }

  std::string getName() const { return this->name; }

  const std::unordered_set<NameSpace*>& getSubNameSpaces() const {
    return subNameSpaces;
  }

  void addSubNameSpace(NameSpace* child) {
    child->parent = this;
    // subNameSpaces.emplace_back(child);
    subNameSpaces.emplace(child);
  }

  void removeSubNameSpace(NameSpace* child) {
    child->parent = nullptr;
    subNameSpaces.erase(child);
  }

  virtual std::string toString() const override {
    return "NameSpace: " + this->name;
  }
};
}  // namespace play