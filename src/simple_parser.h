#ifndef SIMPLE_PARSER_H_
#define SIMPLE_PARSER_H_

#include "token_reader_base.h"
#include "ast_node_base.h"

class SimpleASTNode;
class SimpleAST;


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
  SimpleParser(): ast_builder_() {};

  std::unique_ptr<SimpleAST> Parse(std::string script);

  void DumpAST(SimpleAST ast, std::string indent);

private:
  // std::unique_ptr<SimpleASTNode> Prog(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> ExpressionStatement(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> AssignmentStatement(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> IntDeclare(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> Additive(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> Multiplicative(TokenReaderBase tokens);
  // std::unique_ptr<SimpleASTNode> Primary(TokenReaderBase tokens);
  
  SimpleASTNode* Prog(TokenReaderBase tokens);
  SimpleASTNode* ExpressionStatement(TokenReaderBase tokens);
  SimpleASTNode* AssignmentStatement(TokenReaderBase tokens);
  SimpleASTNode* IntDeclare(TokenReaderBase tokens);
  SimpleASTNode* Additive(TokenReaderBase tokens);
  SimpleASTNode* Multiplicative(TokenReaderBase tokens);
  SimpleASTNode* Primary(TokenReaderBase tokens);

  SimpleAST::Builder ast_builder_;
}

#endif
