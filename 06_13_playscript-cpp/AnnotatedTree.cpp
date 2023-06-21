#include "AnnotatedTree.h"

namespace play {

void AnnotatedTree::log(const std::string& message,
                        const ParserRuleContext* ctx) {
  log(message, CompilationLog::ERROR, ctx);
}
/**
 * 记录编译错误和警告
 * @param message
 * @param type  信息类型，ComplilationLog中的INFO、WARNING和ERROR
 * @param ctx
 */
void AnnotatedTree::log(const std::string& message, int type,
                        const ParserRuleContext* ctx) {
  CompilationLog log;
  ParserRuleContext* mutable_ctx = const_cast<ParserRuleContext*>(ctx);
  log.ctx = mutable_ctx;
  log.message = message;
  log.line = mutable_ctx->getStart()->getLine();
  log.position_in_line = mutable_ctx->getStart()->getStartIndex();
  log.type = type;

  logs.emplace_back(log);

  std::cout << log << std::endl;
}

/**
 * 是否有编译错误
 * @return
 */
bool AnnotatedTree::hasCompilationError() {
  for (CompilationLog& log : logs) {
    if (log.type == CompilationLog::ERROR) return true;
  }
  return false;
}

/**
 * 通过名称查找Variable。逐级Scope查找。
 *
 * @param scope
 * @param idName
 * @return
 */
Variable* AnnotatedTree::lookupVariable(Scope* scope,
                                        const std::string& idName) {
  Variable* rtn = scope->getVariable(idName);
  if (rtn == nullptr && scope->getEnclosingScope() != nullptr) {
    rtn = lookupVariable(rtn->getEnclosingScope(), idName);
  }
  return rtn;
}

/**
 * 通过名称查找Class。逐级Scope查找。
 *
 * @param scope
 * @param idName
 * @return
 */
Class* AnnotatedTree::lookupClass(Scope* scope, const std::string& idName) {
  Class* rtn = scope->getClass(idName);
  if (rtn == nullptr && scope->getEnclosingScope() != nullptr) {
    rtn = lookupClass(rtn->Scope::getEnclosingScope(), idName);
  }
  return rtn;
}

Type* AnnotatedTree::lookupType(const std::string& idName) {
  Type* rtn = nullptr;
  for (Type* type : types) {
    if (type->getName() == idName) {
      rtn = type;
      break;
    }
  }
  return rtn;
}

/**
 * 通过方法的名称和方法签名查找Function。逐级Scope查找。
 *
 * @param scope
 * @param idName
 * @param paramTypes
 * @return
 */
Function* AnnotatedTree::lookupFunction(Scope* scope, const std::string& idName,
                                        const std::vector<Type*>& paramTypes) {
  Function* rtn = scope->getFunction(idName, paramTypes);

  if (rtn == nullptr && scope->getEnclosingScope() != nullptr) {
    rtn = lookupFunction(scope->getEnclosingScope(), idName, paramTypes);
  }
  return rtn;
}

/**
 * 查找函数型变量，逐级查找。
 * @param scope
 * @param idName
 * @param paramTypes
 * @return
 */
Variable* AnnotatedTree::lookupFunctionVariable(
    Scope* scope, const std::string& idName,
    const std::vector<Type*>& paramTypes) {
  Variable* rtn = scope->getFunctionVariable(idName, paramTypes);

  if (rtn == nullptr && scope->getEnclosingScope() != nullptr) {
    rtn =
        lookupFunctionVariable(scope->getEnclosingScope(), idName, paramTypes);
  }
  return rtn;
}

/**
 * 逐级查找函数（或方法）。仅通过名字查找。如果有重名的，返回第一个就算了。//TODO
 * 未来应该报警。
 * @param scope
 * @param name
 * @return
 */
Function* AnnotatedTree::lookupFunction(Scope* scope, const std::string& name) {
  Function* rtn = nullptr;
  if (Class* cls = dynamic_cast<Class*>(scope)) {
    rtn = getMethodOnlyByName(cls, name);
  } else {
    rtn = getFunctionOnlyByName(scope, name);
  }

  if (rtn == nullptr && scope->getEnclosingScope() != nullptr) {
    rtn = lookupFunction(scope->getEnclosingScope(), name);
  }
  return rtn;
}

/**
 * 查找某节点所在的Scope
 * 算法：逐级查找父节点，找到一个对应着Scope的上级节点
 *
 * @param node
 * @return
 */
Scope* AnnotatedTree::enclosingScopeOfNode(ParserRuleContext* node) {
  Scope* rtn = nullptr;
  ParserRuleContext* parent = dynamic_cast<ParserRuleContext*>(node->parent);
  if (parent != nullptr) {
    rtn = node2Scope[parent];
    if (rtn == nullptr) {
      rtn = enclosingScopeOfNode(parent);
    }
  }

  return rtn;
}

/**
 * 包含某节点的函数
 * @param ctx
 * @return
 */
// maybe ParseTree*?
Function* AnnotatedTree::enclosingFunctionOfNode(ParseTree* ctx) {
  if (auto* func_decl_ctx =
          dynamic_cast<PlayScriptParser::FunctionDeclarationContext*>(
              ctx->parent)) {
    return dynamic_cast<Function*>(node2Scope[func_decl_ctx]);
  } else if (ctx->parent == nullptr) {
    return nullptr;
  } else
    return enclosingFunctionOfNode(ctx->parent);
}

/**
 * 包含某节点的类
 * @param ctx
 * @return
 */
Class* AnnotatedTree::enclosingClassOfNode(ParseTree* ctx) {
  if (auto* cls_decl_ctx =
          dynamic_cast<PlayScriptParser::ClassDeclarationContext*>(
              ctx->parent)) {
    return dynamic_cast<Class*>(node2Scope[cls_decl_ctx]);
  } else if (ctx->parent == nullptr) {
    return nullptr;
  } else
    return enclosingClassOfNode(ctx->parent);
}

std::string AnnotatedTree::getScopeTreeString() {
  std::ostringstream oss;
  scopeToString(oss, nameSpace, "");
  return oss.str();
}

// 对于类，需要连父类也查找
Function* AnnotatedTree::getMethodOnlyByName(Class* theClass,
                                             const std::string& name) {
  Function* rtn = getFunctionOnlyByName(theClass, name);

  if (rtn == nullptr && theClass->getParentClass() != nullptr) {
    rtn = getMethodOnlyByName(theClass->getParentClass(), name);
  }
  return rtn;
}

Function* AnnotatedTree::getFunctionOnlyByName(Scope* scope,
                                               const std::string& name) {
  for (Symbol* s : scope->symbols) {
    if (antlrcpp::is<Function*>(s) && s->getName() == name) {
      return dynamic_cast<Function*>(s);
    }
  }
  return nullptr;
}

void AnnotatedTree::scopeToString(std::ostringstream& oss, Scope* scope,
                                  const std::string& indent) {
  oss << indent << scope->toString() << "\n";
  std::string new_indent = indent + "\t";
  for (Symbol* s : scope->symbols) {
    if (Scope* sub_scope = dynamic_cast<Scope*>(s)) {
      scopeToString(oss, sub_scope, new_indent);
    } else {
      oss << new_indent << s->toString() << "\n";
    }
  }
}

}  // namespace play
