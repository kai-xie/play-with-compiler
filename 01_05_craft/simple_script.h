#ifndef SIMPLE_SCRIPT_H_
#define SIMPLE_SCRIPT_H_

#include <map>
#include <string>

class ASTNode;

/**
 * 一个简单的脚本解释器。
 * 所支持的语法，请参见 simple_parser.h
 *
 * 运行脚本：
 * run the binary of simple_script_test.cc
 * 则进入一个REPL界面。你可以依次敲入命令。比如：
 * > 2+3;
 * > int age = 10;
 * > int b;
 * > b = 10*2;
 * > age = age + b;
 * > exit();  //退出REPL界面。
 *
 * 你还可以使用一个参数 -v，让每次执行脚本的时候，都输出AST和整个计算过程。
 *
 */
class SimpleScript {
 public:
  SimpleScript(bool verbose = false) { verbose_ = verbose; }

  int Evaluate(const ASTNode* node, const std::string& indent);

 private:
  static bool verbose_;
  std::map<std::string, int> variables_;
};

#endif  // SIMPLE_SCRIPT_H_
