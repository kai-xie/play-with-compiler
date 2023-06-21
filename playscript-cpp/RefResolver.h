#pragma once

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "Class.h"
#include "DefaultConstructor.h"
#include "PlayScriptBaseListener.h"
#include "PlayScriptParser.h"
#include "Super.h"
#include "This.h"
#include "TypeResolver.h"

namespace play {

/**
 * 语义解析的第三步：引用消解和类型推断
 * 1.解析所有的本地变量引用、函数调用和类成员引用。
 * 2.类型推断：从下而上推断表达式的类型。
 * 这两件事要放在一起做，因为：
 * (1)对于变量，只有做了消解，才能推断出类型来。
 * (2)对于FunctionCall，只有把参数（表达式)的类型都推断出来，才能匹配到正确的函数（方法)。
 * (3)表达式里包含FunctionCall,所以要推导表达式的类型，必须知道是哪个Function，从而才能得到返回值。
 *
 */
class RefResolver : public PlayScriptBaseListener {
 private:
  std::shared_ptr<AnnotatedTree> at;

  //用于把本地变量添加到符号表，并计算类型
  ParseTreeWalker typeResolverWalker = ParseTreeWalker();
  TypeResolver* localVariableEnter = nullptr;

  // this()和super()构造函数留到最后去消解，因为它可能引用别的构造函数，必须等这些构造函数都消解完。
  std::vector<PlayScriptParser::FunctionCallContext*> thisConstructorList;
  std::vector<PlayScriptParser::FunctionCallContext*> superConstructorList;

 public:
  RefResolver(std::shared_ptr<AnnotatedTree> an_tree) : at(an_tree) {
    localVariableEnter = new TypeResolver(at, true);
  }

  virtual void enterVariableDeclarators(
      PlayScriptParser::VariableDeclaratorsContext* ctx) override {
    Scope* scope = at->enclosingScopeOfNode(ctx);
    if (antlrcpp::is<BlockScope*>(scope) || antlrcpp::is<Function*>(scope)) {
      typeResolverWalker.walk(localVariableEnter, ctx);
    }
  }

  virtual void enterPrimary(PlayScriptParser::PrimaryContext* ctx) override {
    Scope* scope = at->enclosingScopeOfNode(ctx);
    Type* type = nullptr;

    // 标识符
    if (ctx->IDENTIFIER()) {
      std::string idName = ctx->IDENTIFIER()->getText();
      Variable* variable = at->lookupVariable(scope, idName);
      if (variable == nullptr) {
        // 看看是不是函数，因为函数可以作为值来传递。这个时候，函数重名没法区分。
        // 因为普通Scope中的函数是不可以重名的，所以这应该是没有问题的。  //TODO
        // 但如果允许重名，那就不行了。
        // TODO 注意，查找function的时候，可能会把类的方法包含进去
        Function* function = at->lookupFunction(scope, idName);
        if (function != nullptr) {
          at->symbolOfNode[ctx] = function;
          type = function;
        } else {
          at->log("unknown variable or function: " + idName, ctx);
        }
      } else {
        at->symbolOfNode[ctx] = variable;
        type = variable->type;
      }
    } else if (ctx->literal()) {  //字面量
      type = at->typeOfNode[ctx->literal()];
    } else if (ctx->expression()) {  // 括号里的表达式
      type = at->typeOfNode[ctx->expression()];
    } else if (ctx->THIS()) {  // this 关键字
                               //找到Class类型的上级Scope
      Class* theClass = at->enclosingClassOfNode(ctx);
      if (theClass != nullptr) {
        This* variable = theClass->getThis();
        at->symbolOfNode[ctx] = variable;
        type = theClass;
      } else {
        at->log("keyword \"this\" can only be used inside a class", ctx);
      }
    } else if (ctx->SUPER()) {  // super
                                // 关键字。看上去跟This关键字的用法完全一样？
      //找到Class类型的上级Scope
      Class* theClass = at->enclosingClassOfNode(ctx);
      if (theClass != nullptr) {
        Super* variable = theClass->getSuper();
        at->symbolOfNode[ctx] = variable;
        type = theClass;
      } else {
        at->log("keyword \"super\" can only be used inside a class", ctx);
      }
    }

    //类型推断、冒泡
    at->typeOfNode[ctx] = type;
  }

  virtual void exitFunctionCall(
      PlayScriptParser::FunctionCallContext* ctx) override {
    // this
    if (ctx->THIS()) {
      thisConstructorList.emplace_back(ctx);
      return;
    }

    if (ctx->SUPER()) {  // super
      superConstructorList.emplace_back(ctx);
      return;
    }

    // TODO 临时代码，支持println
    if (ctx->IDENTIFIER()->getText() == "println") {
      return;
    }

    std::string idName = ctx->IDENTIFIER()->getText();

    // 获得参数类型，这些类型已经在表达式中推断出来
    std::vector<Type*> paramTypes = getParamTypes(ctx);
    bool found = false;

    // 看看是不是点符号表达式调用的，调用的是类的方法
    if (auto* exp =
            dynamic_cast<PlayScriptParser::ExpressionContext*>(ctx->parent)) {
      if (exp->bop && exp->bop->getType() == PlayScriptParser::DOT) {
        Symbol* symbol = at->symbolOfNode[exp->expression(0)];
        if (antlrcpp::is<Variable*>(symbol) &&
            antlrcpp::is<Class*>(dynamic_cast<Variable*>(symbol)->type)) {
          Class* theClass =
              dynamic_cast<Class*>(dynamic_cast<Variable*>(symbol)->type);

          //查找名称和参数类型都匹配的函数。不允许名称和参数都相同，但返回值不同的情况。
          Function* function = theClass->getFunction(idName, paramTypes);
          if (function) {
            found = true;
            at->symbolOfNode[ctx] = function;
            at->typeOfNode[ctx] = function->getReturnType();
          } else {
            Variable* funcVar =
                theClass->getFunctionVariable(idName, paramTypes);
            if (funcVar) {
              found = true;
              at->symbolOfNode[ctx] = funcVar;
              at->typeOfNode[ctx] =
                  dynamic_cast<FunctionType*>(funcVar->type)->getReturnType();
            } else {
              at->log("unable to find method " + idName + " in Class " +
                          theClass->name,
                      exp);
            }
          }
        } else {
          at->log("unable to resolve a class", ctx);
        }
      }
    }

    Scope* scope = at->enclosingScopeOfNode(ctx);

    // 从当前Scope逐级查找函数(或方法)
    if (!found) {
      Function* function = at->lookupFunction(scope, idName, paramTypes);
      if (function) {
        found = true;
        at->symbolOfNode[ctx] = function;
        at->typeOfNode[ctx] = function->getReturnType();
      }
    }

    if (!found) {
      // 看看是不是类的构建函数，用相同的名称查找一个class
      Class* theClass = at->lookupClass(scope, idName);
      if (theClass) {
        Function* function = theClass->findConstructor(paramTypes);
        if (function) {
          found = true;
          at->symbolOfNode[ctx] = function;
        } else if (ctx->expressionList() == nullptr) {
          //如果是与类名相同的方法，并且没有参数，那么就是缺省构造方法
          found = true;
          at->symbolOfNode[ctx] = theClass->getDefaultConstructor();
        } else {
          at->log("unknown class constructor: " + ctx->getText(), ctx);
        }

        // 这次函数调用是返回一个对象
        at->typeOfNode[ctx] = theClass;
      }
    }

    else {
      //看看是不是一个函数型的变量
      Variable* variable =
          at->lookupFunctionVariable(scope, idName, paramTypes);
      if (variable && antlrcpp::is<FunctionType*>(variable->type)) {
        found = true;
        at->symbolOfNode[ctx] = variable;
        at->typeOfNode[ctx] = variable->type;
      } else {
        at->log("unknown function or function variable: " + ctx->getText(),
                ctx);
      }
    }
  }

  virtual void exitExpression(
      PlayScriptParser::ExpressionContext* ctx) override {
    Type* type = nullptr;

    if (ctx->bop && ctx->bop->getType() == PlayScriptParser::DOT) {
      // 这是个左递归，要不断的把左边的节点的计算结果存到node2Symbol，所以要在exitExpression里操作
      Symbol* symbol = at->symbolOfNode[ctx->expression(0)];
      if (auto* sym = dynamic_cast<Variable*>(symbol)) {
        if (auto* theClass = dynamic_cast<Class*>(sym->type)) {
          //引用类的属性
          if (ctx->IDENTIFIER()) {
            std::string idName = ctx->IDENTIFIER()->getText();
            Variable* variable = at->lookupVariable(
                theClass,
                idName);  // 在类的scope里去查找，不需要改变当前的scope
            if (variable) {
              at->symbolOfNode[ctx] = variable;
              type = variable->type;  //类型综合（冒泡)
            } else {
              at->log("unable to find field " + idName + " in Class " +
                          theClass->name,
                      ctx);
            }
          }
          //引用类的方法
          else if (ctx->functionCall()) {
            type = at->typeOfNode[ctx->functionCall()];
          }
        }
      }
      if (type == nullptr) {
        at->log("symbol is not a qualified object: " + symbol->toString(), ctx);
      }
    }
    //变量引用冒泡： 如果下级是一个变量，往上冒泡传递，以便在点符号表达式中使用
    //也包括This和Super的冒泡
    else if (ctx->primary()) {
      Symbol* symbol = at->symbolOfNode[ctx->primary()];
      at->symbolOfNode[ctx] = symbol;
    }

    // 类型推断和综合
    if (ctx->primary()) {
      type = at->typeOfNode[ctx->primary()];
    } else if (ctx->functionCall()) {
      type = at->typeOfNode[ctx->functionCall()];
    } else if (ctx->bop && ctx->expression().size() >= 2) {
      Type* type1 = at->typeOfNode[ctx->expression(0)];
      Type* type2 = at->typeOfNode[ctx->expression(1)];

      switch (ctx->bop->getType()) {
        case PlayScriptParser::ADD:
          if (type1->isType(PrimitiveType::String()) ||
              type2->isType(PrimitiveType::String())) {
            type = PrimitiveType::String();
          } else if (antlrcpp::is<PrimitiveType*>(type1) &&
                     antlrcpp::is<PrimitiveType*>(type2)) {
            //类型“向上”对齐，比如一个int和一个float，取float
            type = PrimitiveType::getUpperType(type1, type2);
          } else {
            at->log(
                "operand should be PrimitiveType for additive and "
                "multiplicative operation",
                ctx);
          }
          break;
        case PlayScriptParser::SUB:
        case PlayScriptParser::MUL:
        case PlayScriptParser::DIV:
          if (antlrcpp::is<PrimitiveType*>(type1) &&
              antlrcpp::is<PrimitiveType*>(type2)) {
            //类型“向上”对齐，比如一个int和一个float，取float
            type = PrimitiveType::getUpperType(type1, type2);
          } else {
            at->log(
                "operand should be PrimitiveType for additive and "
                "multiplicative operation",
                ctx);
          }
          break;
        case PlayScriptParser::EQUAL:
        case PlayScriptParser::NOTEQUAL:
        case PlayScriptParser::LE:
        case PlayScriptParser::LT:
        case PlayScriptParser::GE:
        case PlayScriptParser::GT:
        case PlayScriptParser::AND:
        case PlayScriptParser::OR:
        case PlayScriptParser::BANG:
          type = PrimitiveType::Boolean();
          break;
        case PlayScriptParser::ASSIGN:
        case PlayScriptParser::ADD_ASSIGN:
        case PlayScriptParser::SUB_ASSIGN:
        case PlayScriptParser::MUL_ASSIGN:
        case PlayScriptParser::DIV_ASSIGN:
        case PlayScriptParser::AND_ASSIGN:
        case PlayScriptParser::OR_ASSIGN:
        case PlayScriptParser::XOR_ASSIGN:
        case PlayScriptParser::MOD_ASSIGN:
        case PlayScriptParser::LSHIFT_ASSIGN:
        case PlayScriptParser::RSHIFT_ASSIGN:
        case PlayScriptParser::URSHIFT_ASSIGN:
          type = type1;
          break;
      }
    }
    //类型冒泡
    at->typeOfNode[ctx] = type;
  }

  //对变量初始化部分也做一下类型推断
  virtual void exitVariableInitializer(
      PlayScriptParser::VariableInitializerContext* ctx) override {
    if (ctx->expression()) {
      at->typeOfNode[ctx] = at->typeOfNode[ctx->expression()];
    }
  }

  //根据字面量来推断类型
  virtual void exitLiteral(PlayScriptParser::LiteralContext* ctx) override {
    if (ctx->BOOL_LITERAL()) {
      at->typeOfNode[ctx] = PrimitiveType::Boolean();
    } else if (ctx->CHAR_LITERAL()) {
      at->typeOfNode[ctx] = PrimitiveType::Char();
    } else if (ctx->NULL_LITERAL()) {
      at->typeOfNode[ctx] = PrimitiveType::Null();
    } else if (ctx->STRING_LITERAL()) {
      at->typeOfNode[ctx] = PrimitiveType::String();
    } else if (ctx->integerLiteral()) {
      at->typeOfNode[ctx] = PrimitiveType::Integer();
    } else if (ctx->floatLiteral()) {
      at->typeOfNode[ctx] = PrimitiveType::Float();
    }
  }

  /**
   * 在结束扫描之前，把this()和super()构造函数消解掉
   * @param ctx
   */
  virtual void exitProg(PlayScriptParser::ProgContext* ctx) override {
    for (PlayScriptParser::FunctionCallContext* fcc : thisConstructorList) {
      resolveThisConstructorCall(fcc);
    }

    for (PlayScriptParser::FunctionCallContext* fcc : superConstructorList) {
      resolveSuperConstructorCall(fcc);
    }
  }

 private:
  /**
   * 获得函数的参数列表
   * @param ctx
   * @return
   */
  std::vector<Type*> getParamTypes(PlayScriptParser::FunctionCallContext* ctx) {
    std::vector<Type*> paramTypes;
    if (ctx->expressionList()) {
      for (PlayScriptParser::ExpressionContext* exp :
           ctx->expressionList()->expression()) {
        Type* type = at->typeOfNode[exp];
        paramTypes.emplace_back(type);
      }
    }
    return paramTypes;
  }

  bool firstStatmentInFunction(
      PlayScriptParser::FunctionDeclarationContext* fdx,
      PlayScriptParser::FunctionCallContext* ctx) {
    if (auto* stmt = fdx->functionBody()
                         ->block()
                         ->blockStatements()
                         ->blockStatement(0)
                         ->statement()) {
      if (auto* expr = stmt->expression()) {
        if (expr->functionCall() == ctx) return true;
      }
    }
    return false;
  }

  /**
   * 消解this()构造函数
   * @param ctx
   */
  void resolveThisConstructorCall(PlayScriptParser::FunctionCallContext* ctx) {
    Class* theClass = at->enclosingClassOfNode(ctx);
    if (!theClass) {
      at->log("this() should only be called inside a class constructor", ctx);
    }
    Function* function = at->enclosingFunctionOfNode(ctx);
    if (!function || !function->isConstructor()) {
      at->log("this() should only be called inside a class constructor", ctx);
    }
    //检查是不是构造函数中的第一句
    auto* fdx = dynamic_cast<PlayScriptParser::FunctionDeclarationContext*>(
        function->ctx);
    if (!firstStatmentInFunction(fdx, ctx)) {
      at->log("this() must be first statement in a constructor", ctx);
      return;
    }

    std::vector<Type*> paramTypes = getParamTypes(ctx);
    Function* refered = theClass->findConstructor(paramTypes);
    if (refered) {
      at->symbolOfNode[ctx] = refered;
      at->typeOfNode[ctx] = theClass;
      at->thisConstructorRef[function] = refered;
    } else if (paramTypes.size() == 0) {  // 缺省构造函数
      at->symbolOfNode[ctx] = theClass->getDefaultConstructor();
      at->typeOfNode[ctx] = theClass;
      at->thisConstructorRef[function] = theClass->getDefaultConstructor();
    } else {
      at->log("can not find a constructor matches this()", ctx);
    }
  }

  /**
   * 消解Super()构造函数
   * TODO 对于调用super()是有要求的，比如：
   * (1)必须出现在构造函数的第一行，
   * (2)this()和super不能同时出现，等等。
   * @param ctx
   */
  void resolveSuperConstructorCall(PlayScriptParser::FunctionCallContext* ctx) {
    Class* theClass = at->enclosingClassOfNode(ctx);
    if (!theClass) {
      at->log("super() should only be called inside a class", ctx);
    }

    Function* function = at->enclosingFunctionOfNode(ctx);
    if (!function || !function->isConstructor()) {
      at->log("super() should only be called inside a class constructor", ctx);
    }

    Class* parentClass = theClass->getParentClass();
    if (parentClass) {
      //检查是不是构造函数中的第一句
      auto* fdx = dynamic_cast<PlayScriptParser::FunctionDeclarationContext*>(
          function->ctx);
      if (!firstStatmentInFunction(fdx, ctx)) {
        at->log("super() must be first statement in a constructor", ctx);
        return;
      }

      std::vector<Type*> paramTypes = getParamTypes(ctx);
      Function* refered = parentClass->findConstructor(paramTypes);
      if (refered) {
        at->symbolOfNode[ctx] = refered;
        at->typeOfNode[ctx] = theClass;
        at->thisConstructorRef[function] = refered;
      } else if (paramTypes.size() == 0) {  // 缺省构造函数
        at->symbolOfNode[ctx] = parentClass->getDefaultConstructor();
        at->typeOfNode[ctx] = theClass;
        at->thisConstructorRef[function] = theClass->getDefaultConstructor();
      } else {
        at->log("can not find a constructor matches this()", ctx);
      }

    } else {
      //父类是最顶层的基类。
      // TODO 这里暂时不处理
    }
  }
};
}  // namespace  play
