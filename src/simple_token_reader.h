#ifndef SIMPLE_TOKEN_READER_H_
#define SIMPLE_TOKEN_READER_H_

#include <vector>
#include <memory>

#include "token_reader_base.h"

// The TokenReader should live longer than the TokenBase* returned by Read() or
// Peak()
class SimpleTokenReader : public TokenReaderBase {
 public:
  explicit SimpleTokenReader(std::vector<std::unique_ptr<TokenBase>>& tokens) {
    // Init(tokens);
    for (auto& token : tokens) {
      tokens_.emplace_back(std::move(token));
      // tokens_.emplace_back(std::make_unique<TokenBase>(*token));
    }
    // tokens_ = tokens;
  }

  virtual const TokenBase* Read() override {
    if (IsValidPosition(pos_)) {
      return tokens_[pos_++].get();
    }
    // return std::unique_ptr<TokenBase>(nullptr);
    return nullptr;
  }

  virtual const TokenBase* Peek() const override {
    if (IsValidPosition(pos_)) {
      return tokens_[pos_].get();
    }
    // return std::unique_ptr<TokenBase>(nullptr);
    return nullptr;
  };

  virtual void Unread() override {
    if (pos_ > 0) {
      pos_--;
    }
  }

  virtual int GetPosition() const override { return pos_; }

  virtual void SetPosition(int position) override {
    if (IsValidPosition(position)) {
      pos_ = position;
    }
  }

 private:
  // void Init() {
  //       for (auto& token : tokens) {
  //     // tokens_.emplace_back(std::move(token));
  //     tokens_.emplace_back(std::make_unique<TokenBase>(*token));
  //   }
  // }

  bool IsValidPosition(int position) const {
    return position >= 0 && position < tokens_.size();
  }

  std::vector<std::unique_ptr<TokenBase>> tokens_;

  int pos_ = 0;
};

#endif  // SIMPLE_TOKEN_READER_H_
