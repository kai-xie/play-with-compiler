#ifndef SIMPLE_PARSER_H_
#define SIMPLE_PARSER_H_

#include "token_reader_base.h"
#include "ast_node_base.h"

class SimpleASTNode;
class SimpleAST;

class SimpleParser {
public:
  std::unique_ptr<SimpleAST> Parse(std::string script);

  void DumpAST(SimpleAST ast, std::string indent);
private:
  std::unique_ptr<SimpleASTNode> Prog(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> expressionStatement(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> assignmentStatement(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> intDeclare(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> additive(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> multiplicative(TokenReader tokens);
  std::unique_ptr<SimpleASTNode> primary(TokenReader tokens);
  
}

#endif
