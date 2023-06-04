#pragma once

#include <stack>

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "NameSpace.h"
#include "PlayScriptBaseListener.h"
#include "PlayScriptParser.h"
#include "Scope.h"

namespace play {

// using PlayScriptParser;

class TypeAndScopeScanner : public PlayScriptBaseListener {
 private:
  Ref<AnnotatedTree> at;
  std::stack<Scope*> scopeStack;

 public:
  TypeAndScopeScanner(Ref<AnnotatedTree> an_tree) : at(an_tree) {}

  Scope* pushScope(Scope* scope, ParserRuleContext* ctx) {
    at->node2Scope[ctx] = scope;
    scope->ctx = ctx;
    scopeStack.push(scope);
    return scope;
  }

  // 从栈中弹出scope
  void popScope() { scopeStack.pop(); }

  // 在遍历树的过程中，当前的Scope
  Scope* currentScope() {
    if (scopeStack.size() > 0) {
      return scopeStack.top();
    } else {
      return nullptr;
    }
  }

  void enterProg(PlayScriptParser::ProgContext* ctx) override {
    NameSpace* scope = new NameSpace("", currentScope(), ctx);
    at->nameSpace = scope;
    pushScope(scope, ctx);
  }

  void exitProg(PlayScriptParser::ProgContext* ctx) override { popScope(); }
};

}  // namespace play
