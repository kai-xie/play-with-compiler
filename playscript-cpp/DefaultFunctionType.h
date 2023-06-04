#pragma once
#include <string>
#include <vector>

#include "AntlrPlayScript.h"
#include "FunctionType.h"

namespace play {
class DefaultFunctionType : public FunctionType {
 public:
  std::string name;
  Scope* enclosingScope{nullptr};
  Type* returnType{nullptr};
  std::vector<Type*> paramTypes;

  // 对于未命名的类型，自动赋予名字
  static int nameIndex;

  DefaultFunctionType() { name = "FunctionType" + std::to_string(nameIndex++); }

  virtual std::string getName() const override { return name; }

  virtual Scope* getEnclosingScope() const override { return enclosingScope; }

  virtual Type* getReturnType() override { return returnType; }

  virtual std::vector<Type*> getParamTypes() override { return paramTypes; }

  virtual std::string toString() const override { return "FunctionType"; }

  virtual bool isType(Type* type) const override;

  /**
   * 工具性方法，比较type1是否是type2。
   * 规则：
   * 1.type1的返回值跟type2相等，或者是它的子类型。
   * @param type1
   * @param type2
   * @return
   */
  static bool isType(FunctionType* type1, FunctionType* type2);

  static bool matchParameterTypes(const std::vector<Type*>& lhs,
                                  const std::vector<Type*>& rhs);

  /**
   * 检查改函数是否匹配所需的参数。
   * @param paramTypes
   * @return
   */
  virtual bool matchParameterTypes(
      const std::vector<Type*>& paramTypes) override;
};

}  // namespace play
