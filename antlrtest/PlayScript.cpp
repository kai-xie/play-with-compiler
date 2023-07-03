#include <iostream>
#include <string>
#include <memory>

#include "AntlrTestInc.h"
#include "ASTEvaluator.h"
#include "PlayScriptLexer.h"
#include "PlayScriptParser.h"
// #include "grammar/antlr_files/PlayScriptLexer.h"
// #include "grammar/antlr_files/PlayScriptParser.h"



using namespace antlrtest;
// using namespace 

int main() {
  // additiveExpression
  std::string script = "2 +6/3";

  // assignmentExpression
  // std::string script = "var b = 200";

  // 语法分析
  // lexical analysis
  ANTLRInputStream input(script);
  
  PlayScriptLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  // syntax analysis
  PlayScriptParser parser(&tokens);
  ParseTree* tree = parser.additiveExpression();
  // ParseTree* tree = parser.declaration();

  //print AST
  std::cout << "The list style of ast of : " << script << std::endl;
  std::cout << tree->toStringTree() << std::endl;

  //evaluate
  ASTEvaluator visitor;
  int result = visitor.visit(tree);
  
  std::cout << "\nValue of : " << script << std::endl;
  std::cout << result << std::endl;

  return 0;
}
