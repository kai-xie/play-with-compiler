#pragma once

#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "AntlrPlayScript.h"
#include "Class.h"
#include "CompilationLog.h"
#include "Function.h"
#include "NameSpace.h"
#include "PlayScriptParser.h"
#include "Scope.h"
#include "Type.h"
#include "Variable.h"

namespace play {

// class ParseTree;
// class Type;
// using antlr4::ParserRuleContext;

// class NameSpace;

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
  // AST
  // std::shared_ptr<ParseTree> ast{nullptr};
  ParseTree* ast{nullptr};

  // 解析出来的所有类型，包括类和函数，以后还可以包括数组和枚举。类的方法也作为单独的要素放进去。
  std::vector<Type*> types;

  // AST节点对应的Symbol
  std::unordered_map<ParserRuleContext*, Symbol*> symbolOfNode;

  // AST节点对应的Scope，如for、函数调用会启动新的Scope
  std::unordered_map<ParserRuleContext*, Scope*> node2Scope;

  // 用于做类型推断，每个节点推断出来的类型
  std::unordered_map<ParserRuleContext*, Type*> typeOfNode;

  // 命名空间
  NameSpace* nameSpace{nullptr};  // 全局命名空间

  // 语义分析过程中生成的信息，包括普通信息、警告和错误
  std::vector<CompilationLog> logs;

  // 在构造函数里,引用的this()。第二个函数是被调用的构造函数
  std::unordered_map<Function*, Function*> thisConstructorRef;

  // 在构造函数里,引用的super()。第二个函数是被调用的构造函数
  std::unordered_map<Function*, Function*> superConstructorRef;

  void log(const std::string& message, const ParserRuleContext* ctx);
  /**
   * 记录编译错误和警告
   * @param message
   * @param type  信息类型，ComplilationLog中的INFO、WARNING和ERROR
   * @param ctx
   */
  void log(const std::string& message, int type, const ParserRuleContext* ctx);

  /**
   * 是否有编译错误
   * @return
   */
  bool hasCompilationError();

  /**
   * 通过名称查找Variable。逐级Scope查找。
   *
   * @param scope
   * @param idName
   * @return
   */
  Variable* lookupVariable(Scope* scope, const std::string& idName);

  /**
   * 通过名称查找Class。逐级Scope查找。
   *
   * @param scope
   * @param idName
   * @return
   */
  Class* lookupClass(Scope* scope, const std::string& idName);

  Type* lookupType(const std::string& idName);

  /**
   * 通过方法的名称和方法签名查找Function。逐级Scope查找。
   *
   * @param scope
   * @param idName
   * @param paramTypes
   * @return
   */
  Function* lookupFunction(Scope* scope, const std::string& idName,
                           const std::vector<Type*>& paramTypes);

  /**
   * 查找函数型变量，逐级查找。
   * @param scope
   * @param idName
   * @param paramTypes
   * @return
   */
  Variable* lookupFunctionVariable(Scope* scope, const std::string& idName,
                                   const std::vector<Type*>& paramTypes);

  /**
   * 逐级查找函数（或方法）。仅通过名字查找。如果有重名的，返回第一个就算了。//TODO
   * 未来应该报警。
   * @param scope
   * @param name
   * @return
   */
  Function* lookupFunction(Scope* scope, const std::string& name);

  /**
   * 查找某节点所在的Scope
   * 算法：逐级查找父节点，找到一个对应着Scope的上级节点
   *
   * @param node
   * @return
   */
  Scope* enclosingScopeOfNode(ParserRuleContext* node);

  /**
   * 包含某节点的函数
   * @param ctx
   * @return
   */
  // maybe ParseTree*?
  Function* enclosingFunctionOfNode(ParseTree* ctx);

  /**
   * 包含某节点的类
   * @param ctx
   * @return
   */
  Class* enclosingClassOfNode(ParseTree* ctx);

  std::string getScopeTreeString();

 private:
  // 对于类，需要连父类也查找
  Function* getMethodOnlyByName(Class* theClass, const std::string& name);

  Function* getFunctionOnlyByName(Scope* scope, const std::string& name);

  void scopeToString(std::ostringstream& oss, Scope* scope,
                     const std::string& indent);
};

}  // namespace play
