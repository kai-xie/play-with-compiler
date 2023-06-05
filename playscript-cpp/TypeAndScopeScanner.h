#pragma once

#include <stack>

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "NameSpace.h"
#include "PlayScriptBaseListener.h"
#include "PlayScriptParser.h"
#include "Scope.h"

namespace play {

// using namespace play::PlayScriptParser;

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

  virtual void enterProg(PlayScriptParser::ProgContext* ctx) override {
    NameSpace* scope = new NameSpace("", currentScope(), ctx);
    at->nameSpace = scope;
    pushScope(scope, ctx);
  }

  virtual void exitProg(PlayScriptParser::ProgContext* ctx) override {
    popScope();
  }

  virtual void enterBlock(PlayScriptParser::BlockContext* ctx) override {
    //对于函数，不需要再额外建一个scope
    if (!(antlrcpp::is<PlayScriptParser::FunctionBodyContext*>(ctx->parent))) {
      BlockScope* scope = new BlockScope(currentScope(), ctx);
      currentScope()->addSymbol(scope);
      pushScope(scope, ctx);
    }
  }

  virtual void exitBlock(PlayScriptParser::BlockContext* ctx) override {}

  virtual void enterStatement(
      PlayScriptParser::StatementContext* ctx) override {
    // 为for建立额外的scope
    if (ctx->FOR()) {
      BlockScope* scope = new BlockScope(currentScope(), ctx);
      currentScope()->addSymbol(scope);
      pushScope(scope, ctx);
    }
  }
  virtual void exitStatement(PlayScriptParser::StatementContext* ctx) override {
    if (ctx->FOR()) {
      popScope();
    }
  }

  virtual void enterFunctionDeclaration(
      PlayScriptParser::FunctionDeclarationContext* ctx) override {
    std::string idName = ctx->IDENTIFIER()->getText();

    //注意：目前funtion的信息并不完整，参数要等到TypeResolver.java中去确定。
    Function* function = new Function(idName, currentScope(), ctx);

    at->types.emplace_back(function);
    currentScope()->addSymbol(function);

    // 创建一个新的scope
    pushScope(function, ctx);
  }

  virtual void exitFunctionDeclaration(
      PlayScriptParser::FunctionDeclarationContext* ctx) override {
    popScope();
  }

  virtual void enterClassDeclaration(
      PlayScriptParser::ClassDeclarationContext* ctx) override {
    // 把类的签名存到符号表中，不能跟已有符号名称冲突
    std::string idName = ctx->IDENTIFIER()->getText();

    Class* theClass = new Class(idName, ctx);
    at->types.emplace_back(theClass);

    if (at->lookupClass(currentScope(), idName) != nullptr) {
      at->log("duplicate class name:" + idName,
              ctx);  // 只是报警，但仍然继续解析
    }

    currentScope()->addSymbol(theClass);

    // 创建一个新的scope
    pushScope(theClass, ctx);
  }

  virtual void exitClassDeclaration(
      PlayScriptParser::ClassDeclarationContext* ctx) override {
    popScope();
  }
};

}  // namespace play
