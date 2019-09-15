#ifndef TOKEN_BASE_H_
#define TOKEN_BASE_H_

#include <string>

#include "token_type.h"
/**
 * 一个简单的Token。
 * 只有类型和文本值两个属性。
 * A base class for tokens.
 */

class Token {
 public:
  /**
   * Token的类型
   * @return
   */
  virtual TokenType& Type() = 0;
  virtual const TokenType& Type() const = 0;

  /**
   * Token的文本值
   * @return
   */
  virtual std::string& Text() = 0;
  virtual const std::string& Text() const = 0;

  // virtual ~Token() = default;
};

#endif  // TOKEN_BASE_H_
