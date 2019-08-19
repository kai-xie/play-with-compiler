/**
 * 一个Token流。由Lexer生成。Parser可以从中获取Token。
 */

#ifndef TOKEN_READER_BASE_H_
#define TOKEN_READER_BASE_H_

class TokenReaderBase {
 public:
  /**
   * 返回Token流中下一个Token，并从流中取出。 如果流已经为空，返回null;
   */
  virtual const TokenBase* Read() = 0;

  /**
   * 返回Token流中下一个Token，但不从流中取出。 如果流已经为空，返回null;
   */
  virtual const TokenBase* Peek() const = 0;

  /**
   * Token流回退一步。恢复原来的Token。
   */
  virtual void Unread() = 0;

  /**
   * 获取Token流当前的读取位置。
   * @return
   */
  virtual int GetPosition() const = 0;

  /**
   * 设置Token流当前的读取位置
   * @param position
   */
  virtual void SetPosition(int position) = 0;

  virtual ~TokenReaderBase() = default;
};

#endif
