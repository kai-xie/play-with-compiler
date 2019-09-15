#ifndef SIMPLE_PARSER_H_
#define SIMPLE_PARSER_H_

#include "ast_node_base.h"
#include "simple_ast.h"
#include "token_reader_base.h"

class SimpleASTNode;
// class SimpleAST;

/**
 * 一个简单的语法解析器。
 * 能够解析简单的表达式、变量声明和初始化语句、赋值语句。
 * 它支持的语法规则为：
 *
 * programm -> intDeclare | expressionStatement | assignmentStatement
 * intDeclare -> 'int' Id ( = additive) ';'
 * expressionStatement -> addtive ';'
 * addtive -> multiplicative ( (+ | -) multiplicative)*
 * multiplicative -> primary ( (* | /) primary)*
 * primary -> IntLiteral | Id | (additive)
 */
class SimpleParser {
 public:
  SimpleParser() : ast_builder_(){};

  std::unique_ptr<SimpleAST> Parse(const std::string& script);

  void DumpAST(const ASTNode* node, const std::string& indent) const;

 private:
  // std::unique_ptr<SimpleASTNode> Prog(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> ExpressionStatement(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> AssignmentStatement(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> IntDeclare(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> Additive(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> Multiplicative(TokenReader tokens);
  // std::unique_ptr<SimpleASTNode> Primary(TokenReader tokens);

  SimpleASTNode* Prog(TokenReader* tokens);
  SimpleASTNode* ExpressionStatement(TokenReader* tokens);
  SimpleASTNode* AssignmentStatement(TokenReader* tokens);
  SimpleASTNode* IntDeclare(TokenReader* tokens);
  SimpleASTNode* Additive(TokenReader* tokens);
  SimpleASTNode* Multiplicative(TokenReader* tokens);
  SimpleASTNode* Primary(TokenReader* tokens);

  SimpleAST::Builder ast_builder_;
};

#endif
