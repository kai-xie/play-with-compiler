#ifndef AST_NODE_TYPE_H_
#define AST_NODE_TYPE_H_

#include <iostream>
#include <string>

/**
 * AST node type
 */

// enum class ASTNodeType {
//   Programm,  //程序入口，根节点

//   IntDeclaration,  //整型变量声明
//   ExpressionStmt,  //表达式语句，即表达式后面跟个分号
//   AssignmentStmt,  //赋值语句

//   Primary,      //基础表达式
//   Multiplicative,  //乘法表达式
//   Additive,     //加法表达式

//   Identifier,  //标识符
//   IntLiteral   //整型字面量
// };

#define ASTNODE_TYPE_LIST(MACRO)          \
  MACRO(Program, "Program")              \
  MACRO(IntDeclaration, "IntDeclaration") \
  MACRO(ExpressionStmt, "ExpressionStmt") \
  MACRO(AssignmentStmt, "AssignmentStmt") \
  MACRO(Primary, "Primary")               \
  MACRO(Multiplicative, "Multiplicative") \
  MACRO(Additive, "Additive")             \
  MACRO(Identifier, "Identifier")         \
  MACRO(IntLiteral, "IntLiteral")

enum class ASTNodeType {
#define DECLARE_ENUM(enum_type, enum_string) enum_type,
  ASTNODE_TYPE_LIST(DECLARE_ENUM)
#undef DECLARE_ENUM
};

std::string ASTNodeTypeToString(ASTNodeType ast_node_type);

inline std::ostream& operator<<(std::ostream& os, ASTNodeType ast_node_type) {
  return os << ASTNodeTypeToString(ast_node_type);
}

inline std::string operator+(std::string& str, ASTNodeType ast_node_type) {
  return str + ASTNodeTypeToString(ast_node_type);
}

#endif  //  AST_NODE_TYPE_H_
