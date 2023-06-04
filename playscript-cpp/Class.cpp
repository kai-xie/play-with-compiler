#include "Class.h"

#include "DefaultConstructor.h"
#include "Super.h"
#include "This.h"

namespace play {

Class::Class(const std::string& name, ParserRuleContext* ctx)
    : Scope(name, nullptr, ctx) {
  // this->name = name;
  // this->ctx = const_cast<ParserRuleContext*>(ctx);
  thisRef = new This(this, ctx);
  thisRef->type = this;
}

// Class* Class::getParentClass() const { return parentClass; }

void Class::setParentClass(Class* theClass) {
  parentClass = theClass;

  // 其实superRef引用的也是自己
  superRef = new Super(parentClass, ctx);
  superRef->type = parentClass;
}

/**
 * 是否包含某个Variable，包括自身及父类。
 * @param name
 * @return
 */
Variable* Class::getVariable(const std::string& name) {
  Variable* rtn = Scope::getVariable(name);

  // 这里只检查了一层继承关系
  if (rtn == nullptr && parentClass != nullptr) {
    rtn = parentClass->getVariable(name);  // TODO 是否要检查visibility
  }
  return rtn;
}

/**
 * 是否包含某个Class
 * @param name
 * @return
 */
Class* Class::getClass(const std::string& name) {
  Class* rtn = Scope::getClass(name);

  // 这里只检查了一层继承关系
  if (rtn == nullptr && parentClass != nullptr) {
    rtn = parentClass->getClass(name);  // TODO 是否要检查visibility
  }
  return rtn;
}

/**
 * 找到某个构建函数。不需要往父类去找，在本级找就行了。
 * @param paramTypes
 * @return
 */
Function* Class::findConstructor(const std::vector<Type*>& paramTypes) {
  Function* rtn =
      Scope::getFunction(name, paramTypes);  // TODO 是否要检查visibility?

  return rtn;
}

/**
 * 在自身及父类中找到某个方法
 * @param name
 * @param paramTypes
 * 参数类型列表。该参数不允许为空。如果没有参数，需要传入一个0长度的列表。
 * @return
 */
Function* Class::getFunction(const std::string& name,
                             const std::vector<Type*>& paramTypes) {
  Function* rtn = Scope::getFunction(name, paramTypes);

  // 这里只检查了一层继承关系
  if (rtn == nullptr && parentClass != nullptr) {
    rtn = parentClass->getFunction(name,
                                   paramTypes);  // TODO 是否要检查visibility
  }
  return rtn;
}

Variable* Class::getFunctionVariable(const std::string& name,
                                     const std::vector<Type*>& paramTypes) {
  Variable* rtn = Scope::getFunctionVariable(
      name, paramTypes);  // TODO 是否要检查visibility?

  if (rtn == nullptr && parentClass != nullptr) {
    rtn = parentClass->getFunctionVariable(name, paramTypes);
  }

  return rtn;
}

/**
 * 是否包含某个Symbol。这时候要把父类的成员考虑进来。
 * @param symbol
 * @return
 */
bool Class::containsSymbol(Symbol* symbol) {
  if (symbol == thisRef || symbol == superRef) {
    return true;
  }

  bool rtn = std::any_of(symbols.cbegin(), symbols.cend(),
                         [&](Symbol* s) { return s == symbol; });

  if (!rtn && parentClass != nullptr) {
    rtn = parentClass->containsSymbol(symbol);
  }
  return rtn;
}

/**
 * 当自身是目标类型的子类型的时候，返回true;
 * @param type 目标类型
 * @return
 */
bool Class::isType(Type* type) const {
  if (this == type) return true;  // shortcut

  if (antlrcpp::is<Class*>(type)) {
    return dynamic_cast<Class*>(type)->isAncestor(this);
  }
  return false;
}

/**
 * 本类型是不是另一个类型的祖先类型
 * @param theClass
 * @return
 */
bool Class::isAncestor(const Class* theClass) {
  if (theClass->getParentClass() != nullptr) {
    if (theClass->getParentClass() == this) {
      return true;
    } else {
      return isAncestor(theClass->getParentClass());
    }
  }
  return false;
}

DefaultConstructor* Class::getDefaultConstructor() {
  if (defaultConstructor == nullptr) {
    defaultConstructor = new DefaultConstructor(this->name, this);
  }
  return defaultConstructor;
}

// Initialize the static member variable.
Class* Class::rootClass = new Class("Object", nullptr);

}  // namespace play
