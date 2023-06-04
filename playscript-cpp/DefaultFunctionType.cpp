#include "DefaultFunctionType.h"

namespace play {

int DefaultFunctionType::nameIndex = 1;

bool DefaultFunctionType::isType(Type* type) const {
  if (antlrcpp::is<FunctionType*>(type)) {
    return isType(const_cast<DefaultFunctionType*>(this),
                  dynamic_cast<FunctionType*>(type));
  }
  return false;
}

/**
 * 工具性方法，比较type1是否是type2。
 * 规则：
 * 1.type1的返回值跟type2相等，或者是它的子类型。
 * @param type1
 * @param type2
 * @return
 */
bool DefaultFunctionType::isType(FunctionType* type1, FunctionType* type2) {
  if (type1 == type2) return true;  // shortcut

  if (!type1->getReturnType()->isType(type2->getReturnType())) {
    return false;
  }

  auto paramTypes1 = type1->getParamTypes();
  auto paramTypes2 = type2->getParamTypes();

  return DefaultFunctionType::matchParameterTypes(paramTypes1, paramTypes2);
}

bool DefaultFunctionType::matchParameterTypes(const std::vector<Type*>& lhs,
                                              const std::vector<Type*>& rhs) {
  // 比较每个参数
  if (lhs.size() != rhs.size()) return false;

  bool match = true;
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (!lhs[i]->isType(rhs[i])) {
      match = false;
      break;
    }
  }
  return match;
}

/**
 * 检查改函数是否匹配所需的参数。
 * @param paramTypes
 * @return
 */
bool DefaultFunctionType::matchParameterTypes(
    const std::vector<Type*>& paramTypes) {
  return DefaultFunctionType::matchParameterTypes(this->paramTypes, paramTypes);
}

}  // namespace play
