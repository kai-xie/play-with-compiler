#pragma once

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "PlayScriptBaseListener.h"
#include "PlayScriptParser.h"
#include "VoidType.h"

namespace play {
/**
 * 进行一些语义检查，包括：
 * 01.break 只能出现在循环语句中，或case语句中；
 *
 * 02.return语句
 * 02-01 函数声明了返回值，就一定要有return语句。除非返回值类型是void。
 * 02-02 类的构造函数里如果用到return，不能带返回值。
 * 02-03 return语句只能出现在函数里。
 * 02-04 返回值类型检查 -> (在TypeChecker里做）
 *
 * 03.左值
 * 03-01 标注左值（不标注就是右值)；
 * 03-02 检查表达式能否生成合格的左值。
 *
 * 04.类的声明不能在函数里（TODO 未来应该也可以，只不过对生存期有要求）
 *
 * 05.super()和this()，只能是构造函数中的第一句。  这个在RefResolver中实现了。
 *
 * 06.
 */
class SemanticValidator : public PlayScriptBaseListener {
 private:
  std::shared_ptr<AnnotatedTree> at;

 public:
  SemanticValidator(std::shared_ptr<AnnotatedTree> an_tree) : at(an_tree) {}

  virtual void exitPrimary(PlayScriptParser::PrimaryContext* ctx) override {}

  virtual void exitFunctionCall(
      PlayScriptParser::FunctionCallContext* ctx) override {}

  virtual void exitExpression(
      PlayScriptParser::ExpressionContext* ctx) override {}

  virtual void exitClassDeclaration(
      PlayScriptParser::ClassDeclarationContext* ctx) override {
    // 04 类的声明不能在函数里
    if (at->enclosingFunctionOfNode(ctx)) {
      at->log("can not declare class inside function", ctx);
    }
  }

  virtual void exitFunctionDeclaration(
      PlayScriptParser::FunctionDeclarationContext* ctx) override {
    // 02-01 函数定义了返回值，就一定要有相应的return语句。
    // TODO 更完善的是要进行控制流计算，不是仅仅有一个return语句就行了。
    if (ctx->typeTypeOrVoid()) {
      if (!hasReturnStatement(ctx)) {
        Type* returnType = at->typeOfNode[ctx->typeTypeOrVoid()];
        if (!returnType->isType(VoidType::instance())) {
          at->log("return statment expected in function", ctx);
        }
      }
    }
  }

  virtual void exitVariableDeclarators(
      PlayScriptParser::VariableDeclaratorsContext* ctx) override {
    PlayScriptBaseListener::exitVariableDeclarators(ctx);
  }

  virtual void exitVariableDeclarator(
      PlayScriptParser::VariableDeclaratorContext* ctx) override {
    PlayScriptBaseListener::exitVariableDeclarator(ctx);
  }

  virtual void exitVariableDeclaratorId(
      PlayScriptParser::VariableDeclaratorIdContext* ctx) override {
    PlayScriptBaseListener::exitVariableDeclaratorId(ctx);
  }

  //对变量初始化部分也做一下类型推断
  virtual void exitVariableInitializer(
      PlayScriptParser::VariableInitializerContext* ctx) override {}

  //根据字面量来推断类型
  virtual void exitLiteral(PlayScriptParser::LiteralContext* ctx) override {}

  virtual void exitStatement(PlayScriptParser::StatementContext* ctx) override {
    // 02 类的构造函数不能有返回值
    if (ctx->RETURN()) {
      // 02-03
      Function* function = at->enclosingFunctionOfNode(ctx);
      if (!function) {
        at->log("return statement not in function body", ctx);
      } else if (function->isConstructor() && ctx->expression()) {
        // 02-02 构造函数不能有return语句，如果有，则不能带有返回值
        at->log("can not return a value from constructor", ctx);
      }
    }
    // 01 break 语句
    else if (ctx->BREAK()) {
      if (!checkBreak(ctx)) {
        at->log("break statement not in loop or switch statements", ctx);
      }
    }
  }

 private:
  /**
   * 检查一个函数里有没有return语句。
   * @param ctx
   * @return
   */
  bool hasReturnStatement(ParseTree* ctx) {
    bool rtn = false;
    for (int i = 0; i < ctx->children.size(); ++i) {
      ParseTree* child = ctx->children[i];
      if (antlrcpp::is<PlayScriptParser::StatementContext*>(child) &&
          dynamic_cast<PlayScriptParser::StatementContext*>(child)->RETURN()) {
        rtn = true;
        break;
      } else if (!(antlrcpp::is<PlayScriptParser::FunctionDeclarationContext*>(
                       child) ||
                   antlrcpp::is<PlayScriptParser::ClassDeclarationContext*>(
                       child))) {
        rtn = hasReturnStatement(child);
        if (rtn) {
          break;
        }
      }
    }
    return rtn;
  }

  /**
   * break只能出现在循环语句或switch-case语句里。
   * @param ctx
   * @return
   */
  bool checkBreak(ParseTree* ctx) {
    if (auto* stmt =
            dynamic_cast<PlayScriptParser::StatementContext*>(ctx->parent)) {
      if (stmt->FOR() || stmt->WHILE() ||
          antlrcpp::is<PlayScriptParser::SwitchBlockStatementGroupContext*>(
              stmt)) {
        return true;
      }
    }

    if (ctx->parent == nullptr ||
        antlrcpp::is<PlayScriptParser::FunctionDeclarationContext*>(
            ctx->parent)) {
      return false;
    }

    return checkBreak(ctx->parent);
  }
};

}  // namespace play
