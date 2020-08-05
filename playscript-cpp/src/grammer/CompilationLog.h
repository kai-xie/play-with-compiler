#pragma once

#include <iostream>
#include <string>

namespace play {

class AnnotatedTree;
/**
 * 记录编译过程中产生的信息。
 */
class CompilationLog {
  friend class AnnotatedTree;
  friend ostream& operator<<(ostream& os, const CompilationLog& log);

 public:
  static int INFO = 0;
  static int WARNING = 1;
  static int ERROR = 2;

  // std::string toString() {
  //   return message_ + " @" + line_ + ":" + position_in_line_;
  // }

 protected:
  std::string message_ = "";

  int line_ = -1;
  int position_in_line_ = -1;

  // 相关的AST节点
  ParserRuleContext* ctx_ = nullptr;

  // log的类型，包括信息、警告、错误。
  int type_ = INFO;
};

ostream& operator<<(ostream& os, const CompilationLog& log) {
  os << log.message_ + " @" + log.line_ + ":" + log.position_in_line_;
  return os;
}

}  // namespace play
