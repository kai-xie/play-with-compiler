#pragma once

#include "AntlrPlayScript.h"
// #include "DefaultConstructor.h"
#include "Scope.h"
// #include "Super.h"
// #include "This.h"
#include "Type.h"

namespace play {

class This;
class Super;
class DefaultConstructor;

class Class : public Scope, public Type {
 public:
  // 父类
  Class* parentClass{nullptr};

  // 这个类的This变量
  This* thisRef{nullptr};

  Super* superRef{nullptr};

  DefaultConstructor* defaultConstructor{nullptr};

  Class(const std::string& name, ParserRuleContext* ctx);
  //     {
  //   // this->name = name;
  //   // this->ctx = const_cast<ParserRuleContext*>(ctx);
  //   thisRef = new This(this, ctx);
  //   thisRef->type = this;
  // }

  Class* getParentClass() const { return parentClass; }

  void setParentClass(Class* theClass);

  // 最顶层的基类
  static Class* rootClass;

  This* getThis() { return thisRef; }

  Super* getSuper() { return superRef; }

  virtual std::string toString() const override { return "Class " + name; }

  virtual std::string getName() const override { return name; }

  virtual Scope* getEnclosingScope() const override { return enclosingScope; }

  /**
   * 是否包含某个Variable，包括自身及父类。
   * @param name
   * @return
   */
  Variable* getVariable(const std::string& name) override;

  /**
   * 是否包含某个Class
   * @param name
   * @return
   */
  Class* getClass(const std::string& name) override;

  /**
   * 找到某个构建函数。不需要往父类去找，在本级找就行了。
   * @param paramTypes
   * @return
   */
  Function* findConstructor(const std::vector<Type*>& paramTypes);

  /**
   * 在自身及父类中找到某个方法
   * @param name
   * @param paramTypes
   * 参数类型列表。该参数不允许为空。如果没有参数，需要传入一个0长度的列表。
   * @return
   */
  Function* getFunction(const std::string& name,
                        const std::vector<Type*>& paramTypes) override;

  Variable* getFunctionVariable(const std::string& name,
                                const std::vector<Type*>& paramTypes) override;

  /**
   * 是否包含某个Symbol。这时候要把父类的成员考虑进来。
   * @param symbol
   * @return
   */
  bool containsSymbol(Symbol* symbol) override;

  /**
   * 当自身是目标类型的子类型的时候，返回true;
   * @param type 目标类型
   * @return
   */
  bool isType(Type* type) const override;

  /**
   * 本类型是不是另一个类型的祖先类型
   * @param theClass
   * @return
   */
  bool isAncestor(const Class* theClass);

  DefaultConstructor* getDefaultConstructor();
};

}  // namespace play
