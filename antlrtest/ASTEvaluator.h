#pragma once

#include <string>

// #include "grammar/antlr_files/PlayScriptBaseVisitor.h"
// #include "grammar/antlr_files/PlayScriptParser.h"
// #include "antlr_files/PlayScriptParser.h"
// #include "antlr_files/PlayScriptBaseVisitor.h"
#include "PlayScriptParser.h"
#include "PlayScriptBaseVisitor.h"

namespace antlrtest {

/**
 * 一个Vistor类，只简单的实现了整数的加减乘除。
 */
class ASTEvaluator : public PlayScriptBaseVisitor {
 public:
  virtual antlrcpp::Any visitAdditiveExpression(
      PlayScriptParser::AdditiveExpressionContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    if (ctx->ADD() || ctx->SUB()) {
      int lhs = visitAdditiveExpression(ctx->additiveExpression()).as<int>();
      int rhs = visitMultiplicativeExpression(ctx->multiplicativeExpression())
                    .as<int>();
      if (ctx->ADD()) {
        return lhs + rhs;
      } else {
        return lhs - rhs;
      }
    } else {
      return visitMultiplicativeExpression(ctx->multiplicativeExpression());
    }
  }

  virtual antlrcpp::Any visitMultiplicativeExpression(
      PlayScriptParser::MultiplicativeExpressionContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    if (ctx->MUL() || ctx->DIV() || ctx->MOD()) {
      int lhs = visitMultiplicativeExpression(ctx->multiplicativeExpression())
                    .as<int>();
      int rhs = visitPrimaryExpression(ctx->primaryExpression()).as<int>();
      if (ctx->MUL()) {
        return lhs * rhs;
      } else if (ctx->DIV()) {
        return lhs / rhs;
      } else {
        return lhs % rhs;
      }
    } else {
      return visitPrimaryExpression(ctx->primaryExpression());
    }
  }

  virtual antlrcpp::Any visitPrimaryExpression(
      PlayScriptParser::PrimaryExpressionContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    if (ctx->literal()) {
      return visitLiteral(ctx->literal());
    }
    return 0;
  }

  virtual antlrcpp::Any visitLiteral(
      PlayScriptParser::LiteralContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    if (ctx->IntegerLiteral()) {
      std::cout << "parsing int literal: " << ctx->IntegerLiteral()->getText()
                << std::endl;
      return std::stoi(ctx->IntegerLiteral()->getText());
    }
    return 0;
  }

  virtual antlrcpp::Any visitPrimitiveType(
      PlayScriptParser::PrimitiveTypeContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    std::cout << "parsing primitiveType: " << ctx->getText() << std::endl;
    return 0;
  }

  virtual antlrcpp::Any visitDeclaration(
      PlayScriptParser::DeclarationContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    visitPrimitiveType(ctx->primitiveType());
    std::cout << "parsing identifier in Declaration: "
              << ctx->Identifier()->getText() << std::endl;
    if (ctx->initializer()) {
      return visitInitializer(ctx->initializer());
    }
    return 0;
  }

  virtual antlrcpp::Any visitInitializer(
      PlayScriptParser::InitializerContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    visitAssignmentOperator(ctx->assignmentOperator());
    return visitAssignmentExpression(ctx->assignmentExpression());
  }

  virtual antlrcpp::Any visitAssignmentExpression(
      PlayScriptParser::AssignmentExpressionContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    if (ctx->Identifier()) {
      std::cout << "skip parsing AssignmentExpression with Identifier"
                << std::endl;
      return 0;
    }
    return visitAdditiveExpression(ctx->additiveExpression());
  }

  virtual antlrcpp::Any visitAssignmentOperator(
      PlayScriptParser::AssignmentOperatorContext* ctx) override {
    std::cout << "IN " << __FUNCTION__ << std::endl;
    std::cout << "parsing assignemnt operator: " << ctx->getText() << std::endl;
    return 0;
  }
};
}  // namespace antlrtest
