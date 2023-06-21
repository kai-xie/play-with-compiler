#pragma once

// #include <memory>

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "PlayScriptParser.h"
#include "PrimitiveType.h"
#include "VoidType.h"

namespace play {

/**
 * 第二遍扫描。把变量、类继承、函数声明的类型都解析出来。
 * 也就是所有用到typeTpe的地方。
 *
 * 实际运行时，把变量添加到符号表，是分两步来做的。
 * 第一步，是把类成员变量和函数的参数加进去
 *
 * 第二步，是在变量引用消解的时候再添加。这个时候是边Enter符号表，边消解，会避免变量引用到错误的定义。
 *
 */

class TypeResolver : public PlayScriptBaseListener {
 private:
  Ref<AnnotatedTree> at;

  //是否把本地变量加入符号表
  bool enterLocalVariable = false;

 public:
  TypeResolver(Ref<AnnotatedTree> an_tree) : at(an_tree) {}

  TypeResolver(Ref<AnnotatedTree> an_tree, bool enterLV)
      : at(an_tree), enterLocalVariable(enterLV) {}

  //把类成员变量的声明加入符号表
  virtual void enterVariableDeclaratorID(
      PlayScriptParser::VariableDeclaratorIdContext* ctx) {
    std::string idName = ctx->IDENTIFIER()->getText();
    Scope* scope = at->enclosingScopeOfNode(ctx);

    //第一步只把类的成员变量入符号表。在变量消解时，再把本地变量加入符号表，一边Enter，一边消解。
    if (antlrcpp::is<Class*>(scope) || enterLocalVariable ||
        antlrcpp::is<PlayScriptParser::FormalParameterContext*>(ctx->parent)) {
      Variable* variable = new Variable(idName, scope, ctx);

      //变量查重
      if (scope->getVariable(scope, idName) != nullptr) {
        at->log("Variable or parameter already Declared: " + idName, ctx);
      }
      scope->addSymbol(variable);
      at->symbolOfNode[ctx] = variable;
    }
  }

  //设置所声明的变量的类型
  virtual void exitVariableDeclarators(
      PlayScriptParser::VariableDeclaratorsContext* ctx) override {
    Scope* scope = at->enclosingScopeOfNode(ctx);

    if (antlrcpp::is<Class*>(scope) || enterLocalVariable) {
      // 设置变量类型
      Type* type = dynamic_cast<Type*>(at->typeOfNode[ctx->typeType()]);

      for (PlayScriptParser::VariableDeclaratorContext* child :
           ctx->variableDeclarator()) {
        Variable* variable = dynamic_cast<Variable*>(
            at->symbolOfNode[child->variableDeclaratorId()]);
        variable->type = type;
      }
    }
  }

  //设置函数的返回值类型
  virtual void exitFunctionDeclaration(
      PlayScriptParser::FunctionDeclarationContext* ctx) override {
    Function* function = dynamic_cast<Function*>(at->node2Scope[ctx]);
    if (ctx->typeTypeOrVoid() != nullptr) {
      function->returnType = at->typeOfNode[ctx->typeTypeOrVoid()];
    } else {
      // TODO 如果是类的构建函数，返回值应该是一个类吧？
    }

    //函数查重，检查名称和参数（这个时候参数已经齐了）
    Scope* scope = at->enclosingScopeOfNode(ctx);
    Function* found = Scope::getFunction(scope, function->getName(),
                                         function->getParamTypes());
    if (found && found != function) {
      at->log("Function or method already Declared: " + ctx->getText(), ctx);
    }
  }

  //设置函数的参数的类型，这些参数已经在enterVariableDeclaratorId中作为变量声明了，现在设置它们的类型
  virtual void exitFormalParameter(
      PlayScriptParser::FormalParameterContext* ctx) override {
    // 设置参数类型
    Type* type = at->typeOfNode[ctx->typeType()];
    Variable* variable =
        dynamic_cast<Variable*>(at->symbolOfNode[ctx->variableDeclaratorId()]);
    variable->type = type;

    // 添加到函数的参数列表里
    Scope* scope = at->enclosingScopeOfNode(ctx);
    if (Function* func = dynamic_cast<Function*>(
            scope)) {  // TODO
                       // 从目前的语法来看，只有function才会使用FormalParameter
      func->parameters.emplace_back(variable);
    }
  }

  //设置类的父类
  virtual void enterClassDeclaration(
      PlayScriptParser::ClassDeclarationContext* ctx) override {
    Class* theClass = dynamic_cast<Class*>(at->node2Scope[ctx]);

    //设置父类
    if (ctx->EXTENDS() != nullptr) {
      std::string parentClassName = ctx->typeType()->getText();
      Type* type = at->lookupType(parentClassName);
      if (type && antlrcpp::is<Class*>(type)) {
        theClass->setParentClass(dynamic_cast<Class*>(type));
      } else {
        at->log("unknown class: " + parentClassName, ctx);
      }
    }
  }

  virtual void exitTypeTypeOrVoid(
      PlayScriptParser::TypeTypeOrVoidContext* ctx) override {
    if (ctx->VOID()) {
      at->typeOfNode[ctx] = VoidType::instance();
    } else if (ctx->typeType()) {
      at->typeOfNode[ctx] = at->typeOfNode[ctx->typeType()];
    }
  }

  virtual void exitTypeType(PlayScriptParser::TypeTypeContext* ctx) override {
    // 冒泡，将下级的属性标注在本级
    if (auto* tmp_ctx = ctx->classOrInterfaceType()) {
      at->typeOfNode[ctx] = at->typeOfNode[tmp_ctx];
    } else if (auto* tmp_ctx = ctx->functionType()) {
      at->typeOfNode[ctx] = at->typeOfNode[tmp_ctx];
    } else if (auto* tmp_ctx = ctx->primitiveType()) {
      at->typeOfNode[ctx] = at->typeOfNode[tmp_ctx];
    }
  }

  virtual void enterClassOrInterfaceType(
      PlayScriptParser::ClassOrInterfaceTypeContext* ctx) override {
    if (!ctx->IDENTIFIER().empty()) {
      Scope* scope = at->enclosingScopeOfNode(ctx);
      std::string idName = ctx->getText();
      Class* theClass = at->lookupClass(scope, idName);
      at->typeOfNode[ctx] = theClass;
    }
  }

  virtual void exitFunctionType(
      PlayScriptParser::FunctionTypeContext* ctx) override {
    DefaultFunctionType* functionType = new DefaultFunctionType();
    at->types.emplace_back(functionType);
    at->typeOfNode[ctx] = functionType;
    functionType->returnType = at->typeOfNode[ctx];

    //参数类型
    if (ctx->typeList()) {
      PlayScriptParser::TypeListContext* tlc = ctx->typeList();
      for (PlayScriptParser::TypeTypeContext* ttc : tlc->typeType()) {
        functionType->paramTypes.emplace_back(at->typeOfNode[ttc]);
      }
    }
  }

  virtual void exitPrimitiveType(
      PlayScriptParser::PrimitiveTypeContext* ctx) override {
    Type* type = nullptr;
    if (ctx->BOOLEAN()) {
      type = PrimitiveType::Boolean();
    } else if (ctx->INT()) {
      type = PrimitiveType::Integer();
    } else if (ctx->LONG()) {
      type = PrimitiveType::Long();
    } else if (ctx->FLOAT()) {
      type = PrimitiveType::Float();
    } else if (ctx->DOUBLE()) {
      type = PrimitiveType::Double();
    } else if (ctx->BYTE()) {
      type = PrimitiveType::Byte();
    } else if (ctx->SHORT()) {
      type = PrimitiveType::Short();
    } else if (ctx->CHAR()) {
      type = PrimitiveType::Char();
    } else if (ctx->STRING()) {
      type = PrimitiveType::String();
    }

    at->typeOfNode[ctx] = type;
  }
};

}  // namespace play
