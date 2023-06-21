#include "simple_calculator.h"

#include <memory>

#include "glog/logging.h"

#include "simple_lexer.h"
#include "simple_token_reader.h"

/**
 * 实现一个计算器，但计算的结合性是有问题的。因为它使用了下面的语法规则：
 *
 * additive -> multiplicative | multiplicative + additive
 * multiplicative -> primary | primary * multiplicative
 * //感谢@Void_seT，原来写成+号了，写错了。
 *
 * 递归项在右边，会自然的对应右结合。我们真正需要的是左结合。
 */
int main(int argc, char* argv[]) {
  std::unique_ptr<SimpleCalculator> calculator =
      std::make_unique<SimpleCalculator>();

  //测试变量声明语句的解析
  std::string script = "int a = b+3;";
  std::cout << "parsing variable declaration statement: " << script << std::endl;
  // auto lexer = std::make_unique<SimpleLexer>();
  // std::unique_ptr<SimpleTokenReader> tokens = lexer->Tokenize(script);

  try {
    // SimpleASTNode* node = calculator->IntDeclare(tokens.get());
    auto ast = calculator->Parse(script);
    calculator->DumpAST(ast->root_node(), std::string(""));
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  //测试表达式
  script = "2+3*5";
  std::cout << "\n计算: " + script + "，看上去一切正常。" << std::endl;
  calculator->Evaluate(script);

  //测试语法错误
  script = "2+";
  std::cout << "\n: " + script + "，应该有语法错误。" << std::endl;
  calculator->Evaluate(script);

  script = "2+3+4";
  std::cout << "\n计算: " + script + "，结合性出现错误。" << std::endl;
  calculator->Evaluate(script);

  return 0;
}
