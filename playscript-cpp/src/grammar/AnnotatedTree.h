#ifndef GRAMMER_ANNOTATED_TREE_H_
#define GRAMMER_ANNOTATED_TREE_H_

#include "antlr4-runtime.h"

namespace play {

class ParseTree;
class Type;
/**
 * 注释树。
 * 语义分析的结果都放在这里。跟AST的节点建立关联。包括：
 * 1.类型信息，包括基本类型和用户自定义类型；
 * 2.变量和函数调用的消解；
 * 3.作用域Scope。在Scope中包含了该作用域的所有符号。Variable、Function、Class等都是符号。
 */
class AnnotatedTree {
  friend class PlayScriptCompiler;

 public:
  void log(const std::string& message,const ParserRuleContext* ctx) {
    log(message, CompilationLog::ERROR, ctx);
  }
 protected:
  /**
   * 记录编译错误和警告
   * @param message
   * @param type  信息类型，ComplilationLog中的INFO、WARNING和ERROR
   * @param ctx
   */
 protected
  void log(const std::string& message, int type, const ParserRuleContext* ctx) {
    CompilationLog log;
    log.ctx_ = ctx;
    log.message_ = message;
    log.line_ = ctx->getStart()->getLine();
    log.position_in_line_ = ctx->getStart()->getStartIndex();
    log.type = type;

    logs_.emplace_back(log);

    std::cout << log << std::endl;
  }

  // AST
  std::unique_ptr<ParseTree> ast_;

  // 解析出来的所有类型，包括类和函数，以后还可以包括数组和枚举。类的方法也作为单独的要素放进去。
  std::vector<Type> types_;

  // AST节点对应的Symbol
  std::unordered_map<ParserRuleContext*, Symbol*> symbol_of_node_;
  // AST节点对应的Scope，如for、函数调用会启动新的Scope
  std::unordered_map<ParserRuleContext*, Scope*> scope_of_node_;
  // 用于做类型推断，每个节点推断出来的类型
  std::unordered_map<ParserRuleContext*, Type*> type_of_node_;

  //语义分析过程中生成的信息，包括普通信息、警告和错误
  std::vector<CompilationLog> logs_;

  //在构造函数里,引用的this()。第二个函数是被调用的构造函数
  std::unordered_map<Function*, Function*> this_constructor_ref_;

  //在构造函数里,引用的super()。第二个函数是被调用的构造函数
  std::unordered_map<Function*, Function*> super_constructor_ref_;
};

}  // namespace play

#endif  // GRAMMER_ANNOTATED_TREE_H_
