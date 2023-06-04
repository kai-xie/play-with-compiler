#pragma once

#include <iostream>
#include <string>

// #include "antlr4-runtime.h"
#include "AntlrPlayScript.h"
namespace play {

class AnnotatedTree;

/**
 * 记录编译过程中产生的信息。
 */
class CompilationLog {
  friend class AnnotatedTree;
  friend std::ostream& operator<<(std::ostream& os, const CompilationLog& log) {
    os << log.message + " @" + std::to_string(log.line) + ":" +
              std::to_string(log.position_in_line);
    return os;
  }

 public:
  static constexpr int INFO = 0;
  static constexpr int WARNING = 1;
  static constexpr int ERROR = 2;

  // std::string toString() {
  //   return message_ + " @" + line + ":" + position_in_line_;
  // }

  //  protected:
  std::string message = "";

  int line = -1;
  int position_in_line = -1;

  // 相关的AST节点
  ParserRuleContext* ctx = nullptr;

  // log的类型，包括信息、警告、错误。
  int type = INFO;
};

// std::ostream& operator<<(std::ostream& os, const CompilationLog& log)

}  // namespace play
