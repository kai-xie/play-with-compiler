#ifndef TOKEN_TYPE_H_
#define TOKEN_TYPE_H_

#include <iostream>

/**
 * Token的类型
 */

#define TOKEN_TYPE_LIST(V)    \
  V(Invalid, "InValid")       \
  V(Plus, "Plus")             \
  V(Minus, "Minus")           \
  V(Star, "Star")             \
  V(Slash, "Slash")           \
  V(GE, "GE")                 \
  V(GT, "GT")                 \
  V(EQ, "EQ")                 \
  V(LE, "LE")                 \
  V(LT, "LT")                 \
  V(SemiColon, "SemiColon")   \
  V(LeftParen, "LeftParen")   \
  V(RightParen, "RightParen") \
  V(Assignment, "Assignment") \
  V(If, "If")                 \
  V(Else, "Else")             \
  V(Int, "Int")               \
  V(Identifier, "Identifier") \
  V(IntLiteral, "IntLiteral") \
  V(StringLiteral, "StringLiteral")

enum class TokenType {
#define DECLARE_ENUM(enum_type, enum_string) enum_type,
  TOKEN_TYPE_LIST(DECLARE_ENUM)
#undef DECLARE_ENUM
};

std::string TokenTypeToString(TokenType token_type);

inline std::ostream& operator<<(std::ostream& os, TokenType token_type) {
  return os << TokenTypeToString(token_type);
}

inline std::string operator+(std::string& str, TokenType token_type) {
  return str + TokenTypeToString(token_type);
}

#endif  // TOKEN_TYPE_H_
