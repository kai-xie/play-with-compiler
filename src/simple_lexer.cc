#include "simple_lexer.h"

#include <iostream>
#include <memory>

#include "simple_token_reader.h"
#include "token_base.h"
#include "token_reader_base.h"
#include "token_type.h"

/**
 * Token的一个简单实现。只有类型和文本值两个属性。
 */

class SimpleToken : public Token {
 public:
  virtual TokenType& Type() override final { return type_; }
  virtual const TokenType& Type() const override final { return type_; }

  virtual std::string& Text() override final { return text_; };
  virtual const std::string& Text() const override final { return text_; };

 private:
  TokenType type_ = TokenType::Invalid;
  std::string text_;
};

/**
 * 有限状态机的各种状态。
 */
enum class DfaState {
  Initial,

  If,
  Id_if1,
  Id_if2,
  Else,
  Id_else1,
  Id_else2,
  Id_else3,
  Id_else4,
  Int,
  Id_int1,
  Id_int2,
  Id_int3,
  Id,
  GT,
  GE,

  Assignment,

  Plus,
  Minus,
  Star,
  Slash,

  SemiColon,
  LeftParen,
  RightParen,

  IntLiteral
};

namespace {
bool IsAlpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}

bool IsDigit(char ch) { return ch >= '0' && ch <= '9'; }

//是否是空白字符
bool IsBlank(char ch) { return ch == ' ' || ch == '\t' || ch == '\n'; }

}  // namespace

/**
 * 解析字符串，形成Token。
 * 这是一个有限状态自动机，在不同的状态中迁移。
 * @param code
 * @return
 */
std::unique_ptr<SimpleTokenReader> SimpleLexer::Tokenize(
    const std::string& code) {
  // tokens = new ArrayList<Token>();
  // std::vector<std::unique_ptr<Token>> tokens;
  tokens.clear();
  // CharArrayReader reader = new CharArrayReader(code.toCharArray());

  // token_text = new StringBuffer();
  token_text.clear();
  // std::string token_text;
  // std::unique_ptr<Token> token;
  token.reset(new SimpleToken());
  // int ich = 0;
  char ch = 0;
  int idx = 0;
  int length = code.length();
  DfaState state = DfaState::Initial;
  // try {
  // while ((ich = reader.read()) != -1) {
  while (idx < length) {
    // ch = (char)ich;
    ch = code.at(idx);
    switch (state) {
      case DfaState::Initial:
        state = InitToken(ch);  //重新确定后续状态
        break;
      case DfaState::Id:
        if (IsAlpha(ch) || IsDigit(ch)) {
          token_text += ch;  //保持标识符状态
        } else {
          state = InitToken(ch);  //退出标识符状态，并保存Token
        }
        break;
      case DfaState::GT:
        if (ch == '=') {
          (*token).Type() = TokenType::GE;  //转换成GE
          state = DfaState::GE;
          token_text += ch;
        } else {
          state = InitToken(ch);  //退出GT状态，并保存Token
        }
        break;
      case DfaState::GE:
      case DfaState::Assignment:
      case DfaState::Plus:
      case DfaState::Minus:
      case DfaState::Star:
      case DfaState::Slash:
      case DfaState::SemiColon:
      case DfaState::LeftParen:
      case DfaState::RightParen:
        state = InitToken(ch);  //退出当前状态，并保存Token
        break;
      case DfaState::IntLiteral:
        if (IsDigit(ch)) {
          token_text += ch;  //继续保持在数字字面量状态
        } else {
          state = InitToken(ch);  //退出当前状态，并保存Token
        }
        break;
      case DfaState::Id_int1:
        if (ch == 'n') {
          state = DfaState::Id_int2;
          token_text += ch;
        } else if (IsDigit(ch) || IsAlpha(ch)) {
          state = DfaState::Id;  //切换回Id状态
          token_text += ch;
        } else {
          state = InitToken(ch);
        }
        break;
      case DfaState::Id_int2:
        if (ch == 't') {
          state = DfaState::Id_int3;
          token_text += ch;
        } else if (IsDigit(ch) || IsAlpha(ch)) {
          state = DfaState::Id;  //切换回id状态
          token_text += ch;
        } else {
          state = InitToken(ch);
        }
        break;
      case DfaState::Id_int3:
        if (IsBlank(ch)) {
          (*token).Type() = TokenType::Int;
          state = InitToken(ch);
        } else {
          state = DfaState::Id;  //切换回Id状态
          token_text += ch;
        }
        break;
      default:
        break;
    }
    ++idx;
  }
  // 把最后一个token送进去
  if (token_text.length() > 0) {
    InitToken(ch);
  }
  // }
  // } catch (IOException e) {
  //   e.printStackTrace();
  // }
  auto rtn = std::make_unique<SimpleTokenReader>(tokens);
  return std::move(rtn);
}

/**
 * 打印所有的Token
 * @param token_reader
 */
void SimpleLexer::Dump(SimpleTokenReader& token_reader) {
  std::cout << "text\t\ttype" << std::endl;
  // Token token = null;
  // const std::unique_ptr<Token>& token = token_reader.Read();
  const Token* token = token_reader.Read();
  while (token != nullptr) {
    // System.out.println(token.getText() + "\t\t" + token.getType());
    std::cout << token->Text() + "\t\t" << token->Type() << std::endl;
    token = token_reader.Read();
  }
}

/**
 * 有限状态机进入初始状态。
 * 这个初始状态其实并不做停留，它马上进入其他状态。
 * 开始解析的时候，进入初始状态；某个Token解析完毕，也进入初始状态，在这里把Token记下来，然后建立一个新的Token。
 * @param ch
 * @return
 */
DfaState SimpleLexer::InitToken(char ch) {
  if (token_text.length() > 0) {
    // Save the current token
    (*token).Text() = token_text;
    tokens.emplace_back(std::move(token));

    // TODO: fix the following code.
    // token_text = new StringBuffer();
    token_text.clear();
    token.reset(new SimpleToken());
  }

  DfaState newState = DfaState::Initial;
  if (IsAlpha(ch)) {  //第一个字符是字母
    if (ch == 'i') {
      newState = DfaState::Id_int1;
    } else {
      newState = DfaState::Id;  //进入Id状态
    }
    (*token).Type() = TokenType::Identifier;
    token_text += ch;
  } else if (IsDigit(ch)) {  //第一个字符是数字
    newState = DfaState::IntLiteral;
    (*token).Type() = TokenType::IntLiteral;
    token_text += ch;
  } else if (ch == '>') {  //第一个字符是>
    newState = DfaState::GT;
    (*token).Type() = TokenType::GT;
    token_text += ch;
  } else if (ch == '+') {
    newState = DfaState::Plus;
    (*token).Type() = TokenType::Plus;
    token_text += ch;
  } else if (ch == '-') {
    newState = DfaState::Minus;
    (*token).Type() = TokenType::Minus;
    token_text += ch;
  } else if (ch == '*') {
    newState = DfaState::Star;
    (*token).Type() = TokenType::Star;
    token_text += ch;
  } else if (ch == '/') {
    newState = DfaState::Slash;
    (*token).Type() = TokenType::Slash;
    token_text += ch;
  } else if (ch == ';') {
    newState = DfaState::SemiColon;
    (*token).Type() = TokenType::SemiColon;
    token_text += ch;
  } else if (ch == '(') {
    newState = DfaState::LeftParen;
    (*token).Type() = TokenType::LeftParen;
    token_text += ch;
  } else if (ch == ')') {
    newState = DfaState::RightParen;
    (*token).Type() = TokenType::RightParen;
    token_text += ch;
  } else if (ch == '=') {
    newState = DfaState::Assignment;
    (*token).Type() = TokenType::Assignment;
    token_text += ch;
  } else {
    newState = DfaState::Initial;  // skip all unknown patterns
  }
  return newState;
}
