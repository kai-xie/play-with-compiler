#ifndef SIMPLE_CALCULATOR_H_
#define SIMPLE_CALCULATOR_H_

#include <memory>
#include <string>

#include "ast_node_base.h"
#include "simple_ast.h"
#include "token_reader_base.h"

class ASTNode;
class TokenReader;
class SimpleASTNode;

/**
 * 实现一个计算器，但计算的结合性是有问题的。因为它使用了下面的语法规则：
 *
 * additive -> multiplicative | multiplicative + additive
 * multiplicative -> primary | primary * multiplicative
 *
 * 递归项在右边，会自然的对应右结合。我们真正需要的是左结合。
 */
class SimpleCalculator {
 public:
  /**
   * 执行脚本，并打印输出AST和求值过程。
   * @param script
   */
  void Evaluate(const std::string& script);

  /**
   * 解析脚本，并返回根节点
   * @param code
   * @return
   * @throws Exception
   */
  std::unique_ptr<SimpleAST> Parse(const std::string& code);

  /**
   * 对某个AST节点求值，并打印求值过程。
   * @param node
   * @param indent  打印输出时的缩进量，用tab控制
   * @return
   */
  int Evaluate(ASTNode* node, const std::string& indent);

  /**
   * 语法解析：根节点
   * @return
   * @throws Exception
   */
  SimpleASTNode* Prog(TokenReader* tokens);

  /**
   * 整型变量声明语句，如：
   * int a;
   * int b = 2*3;
   *
   * @return
   * @throws Exception
   */
  SimpleASTNode* IntDeclare(TokenReader* tokens);

  /**
   * 语法解析：加法表达式
   * @return
   * @throws Exception
   */
  SimpleASTNode* Additive(TokenReader* tokens);

  /**
   * 语法解析：乘法表达式
   * @return
   * @throws Exception
   */
  SimpleASTNode* Multiplicative(TokenReader* tokens);

  /**
   * 语法解析：基础表达式
   * @return
   * @throws Exception
   */
  SimpleASTNode* Primary(TokenReader* tokens);

  void DumpAST(const ASTNode* node, const std::string& indent) const;

 private:
  SimpleAST::Builder ast_builder_;
};

#endif  // SIMPLE_CALCULATOR_H_
