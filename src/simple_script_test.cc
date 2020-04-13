#include "simple_script.h"

#include <cstring>
#include <iostream>
#include <memory>

#include "simple_parser.h"

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
int main(int argc, char* argv[]) {
  bool verbose = false;
  if (argc > 1 && strcmp(argv[1], "-v") == 0) {
    verbose = true;
    std::cout << "verbose mode" << std::endl;
  }
  std::cout << "Simple script language!" << std::endl;
  std::cout << "(enter 'exit();' to exit)" << std::endl;

  std::unique_ptr<SimpleParser> parser = std::make_unique<SimpleParser>();
  std::unique_ptr<SimpleScript> script =
      std::make_unique<SimpleScript>(verbose);
  std::string input;
  std::string script_text("");
  std::cout << "\n>";

  while (true) {
    try {
      getline(std::cin, input);
      if (input == "exit();" || (script_text + input) == "exit();") {
        std::cout << "good bye!" << std::endl;
        break;
      }

      script_text += input;
      // std::cout << "script_text: " << script_text << std::endl;
      if (input.compare(input.size() - 1, 1, ";") ==
          0) {  // input ends wiht ";"
        auto tree = parser->Parse(script_text);
        if (verbose) {
          parser->DumpAST(tree->root_node(), "");
        }
        script->Evaluate(tree->root_node(), "");
        std::cout << "\n>";
        script_text = "";
      }
    } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      std::cout << "\n>";
      script_text = "";
    }
  }
  return 0;
}