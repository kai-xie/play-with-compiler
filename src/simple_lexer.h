#ifndef SIMPLE_LEXER_H_
#define SIMPLE_LEXER_H_

#include <memory>
#include <string>
#include <vector>

#include "token_base.h"

class TokenBase;
// class TokenReaderBase;
class SimpleTokenReader;
enum class DfaState;

/**
 * 一个简单的手写的词法分析器。
 * 能够为后面的简单计算器、简单脚本语言产生Token。
 */
class SimpleLexer {
 public:
  /**
   * 解析字符串，形成Token。
   * 这是一个有限状态自动机，在不同的状态中迁移。
   * @param code
   * @return
   */
  std::unique_ptr<SimpleTokenReader> Tokenize(const std::string& code);

  /**
   * 打印所有的Token
   * @param token_reader
   */
  void Dump(SimpleTokenReader& token_reader);

 private:
  /**
   * 有限状态机进入初始状态。
   * 这个初始状态其实并不做停留，它马上进入其他状态。
   * 开始解析的时候，进入初始状态；某个Token解析完毕，也进入初始状态，在这里把Token记下来，然后建立一个新的Token。
   * @param ch
   * @return
   */
  DfaState InitToken(char ch);

  //下面几个变量是在解析过程中用到的临时变量,如果要优化的话，可以塞到方法里隐藏起来
  //临时保存token的文本
  std::string token_text;

  //保存解析出来的Token
  std::vector<std::unique_ptr<TokenBase>> tokens;

  // 当前正在解析的Token
  std::unique_ptr<TokenBase> token;
  // SimpleToken token;
};

#endif  // SIMPLE_LEXER_H_
