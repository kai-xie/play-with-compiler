#pragma once

#include <string>

#include "Type.h"

namespace play {
class PrimitiveType : public Type {
 private:
  std::string name;

  // 没有公共的构造方法
  PrimitiveType(const std::string& name) { this->name = name; }

 public:
  std::string getName() const override { return name; }

  Scope* getEnclosingScope() const override { return nullptr; }

  bool isType(Type* type) const override { return this == type; }

  // 把常见的基础数据类型都定义出来
  static PrimitiveType Integer = PrimitiveType("Integer");
  static PrimitiveType Long = PrimitiveType("Long");
  static PrimitiveType Float = PrimitiveType("Float");
  static PrimitiveType Double = PrimitiveType("Double");
  static PrimitiveType Boolean = PrimitiveType("Boolean");
  static PrimitiveType Byte = PrimitiveType("Byte");
  static PrimitiveType Char = PrimitiveType("Char");
  static PrimitiveType Short = PrimitiveType("Short");
  static PrimitiveType String =
      PrimitiveType("String");  // 增加String为基础类型
  static PrimitiveType Null = PrimitiveType("null");

  bool operator==(const PrimitiveType& other) {
    if (other.getName() == this->getName()) return true;
    return false;
  }

  /**
   * 计算两个类型中比较“高”的一级，比如int和long相加，要取long
   *
   * @param type1
   * @param type2
   * @return
   */
  static PrimitiveType* getUpperType(Type* type1, Type* type2) {
    PrimitiveType* type = nullptr;
    if (*type1 == PrimitiveType::String || *type2 == PrimitiveType::String) {
      type = &PrimitiveType::String;
    } else if (*type1 == PrimitiveType::Double ||
               *type2 == PrimitiveType::Double) {
      type = &PrimitiveType::Double;
    } else if (*type1 == PrimitiveType::Float ||
               *type2 == PrimitiveType::Float) {
      type = &PrimitiveType::Float;
    } else if (*type1 == PrimitiveType::Long || *type2 == PrimitiveType::Long) {
      type = &PrimitiveType::Long;
    } else if (*type1 == PrimitiveType::Integer ||
               *type2 == PrimitiveType::Integer) {
      type = &PrimitiveType::Integer;
    } else if (*type1 == PrimitiveType::Short ||
               *type2 == PrimitiveType::Short) {
      type = &PrimitiveType::Short;
    } else {
      type = &PrimitiveType::Byte;
    }
    return type;
  }

  /**
   * 某个类型是不是数值型的（以便进行数值型运算）
   *
   * @param type
   * @return
   */
  static bool isNumeric(Type* type) {
    if (*type == PrimitiveType.Byte || *type == PrimitiveType.Short ||
        *type == PrimitiveType.Integer || *type == PrimitiveType.Long ||
        *type == PrimitiveType.Float || *type == PrimitiveType.Double) {
      return true;
    }
    return false;
  }
};
}  // namespace play
