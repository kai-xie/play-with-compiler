#pragma once

#include <unordered_set>

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "Function.h"
#include "Variable.h"

namespace play {
template <typename T>
using uset = std::unordered_set<T>;

class ClosureAnalyzer {
 private:
  std::shared_ptr<AnnotatedTree> at;

 public:
  ClosureAnalyzer(std::shared_ptr<AnnotatedTree> an_tree) : at(an_tree) {}

  /**
   * 对所有的函数做闭包分析。
   * 只做标准函数的分析，不做类的方法的分析。
   */
  void analyzeClosures() {
    for (Type* type : at->types) {
      Function* func = dynamic_cast<Function*>(type);
      if (!func) continue;
      if (func->isMethod()) continue;
      uset<Variable*> set = calcClosureVariables(func);
      if (set.size() > 0) {
        func->closureVariables = set;
      }
    }
  }

 private:
  /**
   * 看看node1是不是node2的祖先
   * @param node1
   * @param node2
   */
  bool isAncestor(ParseTree* node1, ParseTree* node2) {
    if (node2->parent == nullptr) {
      return false;
    } else if (node2->parent == node1) {
      return true;
    } else {
      return isAncestor(node1, node2->parent);
    }
  }

  /**
   * 被一个Scope（包括下级Scope）内部的代码所引用的所有变量的集合
   * @param scope
   * @return
   */
  uset<Variable*> variableReferedByScope(Scope* scope) {
    uset<Variable*> rtn;
    ParserRuleContext* scopeNode = scope->ctx;

    //扫面所有的符号引用。这对于大的程序性能不够优化，因为符号表太大。
    for (auto [node, symbol] : at->symbolOfNode) {
      if (antlrcpp::is<Variable*>(symbol) && isAncestor(scopeNode, node)) {
        rtn.insert(dynamic_cast<Variable*>(symbol));
      }
    }

    return rtn;
  }

  /**
   * 在一个Scope（及）下级Scope中声明的所有变量的集合
   * @param scope
   * @return
   */
  uset<Variable*> variablesDeclaredUnderScope(Scope* scope) {
    uset<Variable*> rtn;
    for (Symbol* symbol : scope->symbols) {
      if (auto* sym = dynamic_cast<Variable*>(symbol)) {
        rtn.insert(sym);
      } else if (auto* ascope = dynamic_cast<Scope*>(symbol)) {
        rtn.merge(variablesDeclaredUnderScope(ascope));
      }
    }

    return rtn;
  }

  /**
   * 为某个函数计算闭包变量，也就是它所引用的外部环境变量。
   * 算法：计算所有的变量引用，去掉内部声明的变量，剩下的就是外部的。
   * @param function
   * @return
   */
  uset<Variable*> calcClosureVariables(Function* function) {
    uset<Variable*> refered = variableReferedByScope(function);
    uset<Variable*> declared = variablesDeclaredUnderScope(function);
    for (Variable* v : declared) {
      refered.erase(v);
    }
    return refered;
  }
};
}  // namespace play
