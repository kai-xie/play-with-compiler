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
  virtual std::string getName() const override { return name; }

  virtual Scope* getEnclosingScope() const override { return nullptr; }

  virtual bool isType(Type* type) const override { return this == type; }

  virtual std::string toString() const override {
    return "PrimitiveType " + this->name;
  }

  // 把常见的基础数据类型都定义出来
  // static PrimitiveType Integer = PrimitiveType("Integer");
  // static PrimitiveType Long = PrimitiveType("Long");
  // static PrimitiveType Float = PrimitiveType("Float");
  // static PrimitiveType Double = PrimitiveType("Double");
  // static PrimitiveType Boolean = PrimitiveType("Boolean");
  // static PrimitiveType Byte = PrimitiveType("Byte");
  // static PrimitiveType Char = PrimitiveType("Char");
  // static PrimitiveType Short = PrimitiveType("Short");
  // static PrimitiveType String =
  //     PrimitiveType("String");  // 增加String为基础类型
  // static PrimitiveType Null = PrimitiveType("null");

  static PrimitiveType* Integer() {
    static PrimitiveType* pt = new PrimitiveType("Integer");
    return pt;
  }
  static PrimitiveType* Long() {
    static PrimitiveType* pt = new PrimitiveType("Long");
    return pt;
  }
  static PrimitiveType* Float() {
    static PrimitiveType* pt = new PrimitiveType("Float");
    return pt;
  }
  static PrimitiveType* Double() {
    static PrimitiveType* pt = new PrimitiveType("Double");
    return pt;
  }
  static PrimitiveType* Boolean() {
    static PrimitiveType* pt = new PrimitiveType("Boolean");
    return pt;
  }
  static PrimitiveType* Byte() {
    static PrimitiveType* pt = new PrimitiveType("Byte");
    return pt;
  }
  static PrimitiveType* Char() {
    static PrimitiveType* pt = new PrimitiveType("Char");
    return pt;
  }
  static PrimitiveType* Short() {
    static PrimitiveType* pt = new PrimitiveType("Short");
    return pt;
  }
  static PrimitiveType* String() {
    static PrimitiveType* pt = new PrimitiveType("String");
    return pt;
  }
  static PrimitiveType* Null() {
    static PrimitiveType* pt = new PrimitiveType("null");
    return pt;
  }

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
    if (type1->isType(PrimitiveType::String()) ||
        type2->isType(PrimitiveType::String())) {
      type = PrimitiveType::String();
    } else if (type1->isType(PrimitiveType::Double()) ||
               type2->isType(PrimitiveType::Double())) {
      type = PrimitiveType::Double();
    } else if (type1->isType(PrimitiveType::Float()) ||
               type2->isType(PrimitiveType::Float())) {
      type = PrimitiveType::Float();
    } else if (type1->isType(PrimitiveType::Long()) ||
               type2->isType(PrimitiveType::Long())) {
      type = PrimitiveType::Long();
    } else if (type1->isType(PrimitiveType::Integer()) ||
               type2->isType(PrimitiveType::Integer())) {
      type = PrimitiveType::Integer();
    } else if (type1->isType(PrimitiveType::Short()) ||
               type2->isType(PrimitiveType::Short())) {
      type = PrimitiveType::Short();
    } else {
      type = PrimitiveType::Byte();
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
    if (type->isType(PrimitiveType::Byte()) || type->isType(PrimitiveType::Short()) ||
        type->isType(PrimitiveType::Integer()) || type->isType(PrimitiveType::Long()) ||
        type->isType(PrimitiveType::Float()) || type->isType(PrimitiveType::Double())) {
      return true;
    }
    return false;
  }
};
}  // namespace play
