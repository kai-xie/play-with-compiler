#include "simple_parser.h"

#include <iostream>
#include <vector>

#include "absl/container/flat_hash_set.h"
#include "glog/logging.h"

#include "ast_node_base.h"
#include "ast_node_type.h"
#include "map_util.h"
#include "simple_lexer.h"
#include "token_base.h"
#include "token_type.h"

#include "simple_token_reader.h"

/**
 * 一个简单的AST节点。
 * 属性包括：类型、文本值、父节点、子节点。
 */
class SimpleASTNode : public ASTNode {
 public:
  SimpleASTNode(ASTNodeType node_type, const std::string& text)
      : node_type_(node_type), text_(text) {}

  virtual const ASTNode* GetParent() const override { return parent_; }

  //子节点
  virtual const std::vector<ASTNode*> GetChildren() const override {
    return children_;
  }

  // AST类型
  virtual ASTNodeType GetType() const override { return node_type_; }

  //文本值
  virtual const std::string& GetText() const override { return text_; }

  void AddChild(SimpleASTNode* child) {
    if (!ContainsKey(children_set_, child)) {
      children_.emplace_back(child);
      children_set_.insert(child);
      child->parent_ = this;
    }
  }

 private:
  SimpleASTNode* parent_;

  std::vector<ASTNode*> children_;
  absl::flat_hash_set<const ASTNode*> children_set_;
  // std::set<ASTNode*> children_set_;

  ASTNodeType node_type_;
  std::string text_;
};

/**
 * 一个简单的语法解析器。
 * 能够解析简单的表达式、变量声明和初始化语句、赋值语句。
 * 它支持的语法规则为：
 *
 * programm -> intDeclare | expressionStatement | assignmentStatement
 * intDeclare -> 'int' Id ( = additive) ';'
 * expressionStatement -> addtive ';'
 * addtive -> multiplicative ( (+ | -) multiplicative)*
 * multiplicative -> primary ( (* | /) primary)*
 * primary -> IntLiteral | Id | (additive)
 */

std::unique_ptr<SimpleAST> SimpleParser::Parse(const std::string& script) {
  std::unique_ptr<SimpleLexer> lexer = std::make_unique<SimpleLexer>();
  auto simple_token_reader = lexer->Tokenize(script);
  // TokenReader* tokens =
  // dynamic_cast<TokenReader*>(simple_token_reader.get());
  TokenReader* tokens = simple_token_reader.get();
  ASTNode* root_node = static_cast<SimpleASTNode*>(Prog(tokens));
  return ast_builder_.Build(root_node);
  // return root_node;
}

template <typename T>
inline T* AddNodeToBuilder(SimpleAST::Builder& builder,
                           std::unique_ptr<T>&& node) {
  // T* ret = node.get();
  // builder.AddASTNode(node);
  // return ret;
  return builder.AddASTNode(node);
}

namespace {
inline bool CheckTokenType(const Token* ptoken, const TokenType type) {
  return (ptoken != nullptr) && (ptoken->Type() == type);
}

template <typename T>
T* LogIfNullAndReturn(T* ptr, const std::string& msg) {
  if (ptr == nullptr) {
    VLOG(1) << "NOT found: " << msg;
  } else {
    VLOG(1) << "Found: " << msg;
  }
  return ptr;
}
}  // namespace

/**
 * AST的根节点，解析的入口。
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::Prog(TokenReader* tokens) {
  VLOG(1) << "parsing Prog...";
  std::unique_ptr<SimpleASTNode> node =
      std::make_unique<SimpleASTNode>(ASTNodeType::Program, "pwc");
  while (tokens->Peek() != nullptr) {
    // std::unique_ptr<SimpleASTNode> child = IntDeclare(tokens);
    SimpleASTNode* child = IntDeclare(tokens);
    if (nullptr == child) {
      child = ExpressionStatement(tokens);
    }
    if (nullptr == child) {
      child = AssignmentStatement(tokens);
    }
    // CHECK(child) << "unknown statement";
    if (child) {
      node->AddChild(child);
    } else {
      throw std::logic_error("unknown statement");
    }
  }
  SimpleASTNode* ret =
      static_cast<SimpleASTNode*>(ast_builder_.AddASTNode(std::move(node)));
  return LogIfNullAndReturn(ret, "Prog");
  // return ret;
};

/**
 * 表达式语句，即表达式后面跟个分号。
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::ExpressionStatement(TokenReader* tokens) {
  VLOG(1) << "parsing Expression...";
  int pos = tokens->GetPosition();
  SimpleASTNode* node = Additive(tokens);
  if (node != nullptr) {
    const Token* ptoken = tokens->Peek();
    if (CheckTokenType(ptoken, TokenType::SemiColon)) {
      tokens->Read();
      VLOG(1) << "IS Expression ...";
    } else {
      node = nullptr;
      tokens->SetPosition(pos);  // 回溯
      VLOG(1) << "Not Expression ...";
    }
  }
  return LogIfNullAndReturn(node, "ExpressionStatement");
  // return node;  // //直接返回子节点，简化了AST。
}

/**
 * 赋值语句，如age = 10*2;
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::AssignmentStatement(TokenReader* tokens) {
  VLOG(1) << "parsing AssignmentStatement...";
  bool assignment_found = false;
  std::unique_ptr<SimpleASTNode> node;
  const Token* ptoken =
      tokens->Peek();  // peek to see if the next one is Identifier
  if (CheckTokenType(ptoken, TokenType::Identifier)) {
    ptoken = tokens->Read();  // read out the token
    node = absl::WrapUnique(
        new SimpleASTNode(ASTNodeType::AssignmentStmt, ptoken->Text()));
    ptoken = tokens->Peek();
    if (CheckTokenType(ptoken, TokenType::Assignment)) {
      tokens->Read();  // get the "="
      SimpleASTNode* child = Additive(tokens);
      // Error, no valid expression on the right of the "="
      // CHECK(child) << "invalide assignment statement, expecting an
      // expression";
      if (child == nullptr) {
        throw std::logic_error(
            "invalide assignment statement, expecting an expression");
      } else {
        node->AddChild(child);    // add child node
        ptoken = tokens->Peek();  // peek to see whether the following is ";"
        // CHECK(CheckTokenType(ptoken, TokenType::SemiColon))
        //     << "invalid statement, expecting semicolon";
        if (CheckTokenType(ptoken, TokenType::SemiColon)) {
          tokens->Read();
          assignment_found = true;
        } else {
          throw std::logic_error("invalid statement, expecting semicolon");
        }
      }
    } else {
      tokens->Unread();
      node.reset(nullptr);
    }
  }
  SimpleASTNode* ret =
      static_cast<SimpleASTNode*>(ast_builder_.AddASTNode(std::move(node)));

  return LogIfNullAndReturn(ret, "AssignmentStatement");
  // if (assignment_found && node) {
  //   VLOG(1) << "IS AssignmentStatement ...";
  //   return static_cast<SimpleASTNode*>(
  //       ast_builder_.AddASTNode(std::move(node)));
  // }
  // VLOG(1) << "NOT AssignmentStatement ...";
  // return nullptr;  // no assignment statement found
}

/**
 * 整型变量声明，如：
 * int a;
 * int b = 2*3;
 *
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::IntDeclare(TokenReader* tokens) {
  VLOG(1) << "parsing IntDeclare...";
  bool int_declare_found = false;
  std::unique_ptr<SimpleASTNode> node;
  const Token* ptoken = tokens->Peek();
  if (CheckTokenType(ptoken, TokenType::Int)) {
    VLOG(2) << "  see 'int'...";
    ptoken = tokens->Read();  // get "int"
    if (tokens->Peek()->Type() == TokenType::Identifier) {
      ptoken = tokens->Read();
      VLOG(2) << "  see 'identifier': " << ptoken->Text();
      node = absl::WrapUnique(
          new SimpleASTNode(ASTNodeType::IntDeclaration, ptoken->Text()));
      ptoken = tokens->Peek();
      if (CheckTokenType(ptoken, TokenType::Assignment)) {
        tokens->Read();  // get the "="
        VLOG(2) << "  see '=' ...";
        SimpleASTNode* child = Additive(tokens);
        // CHECK(child)
        //     << "invalide variable initialization, expecting an expression";
        if (child == nullptr) {
          throw std::logic_error(
              "invalide variable initialization, expecting an expression");
        }
        node->AddChild(child);
      }
    } else {
      // CHECK(false) << "variable name expected";
      throw std::logic_error("variable name expected");
    }

    if (node) {
      ptoken = tokens->Peek();
      if (CheckTokenType(ptoken, TokenType::SemiColon)) {
        tokens->Read();
        int_declare_found = true;
      } else {
        // CHECK(false) << "invalid statement, expecting semicolon";
        throw std::logic_error("invalid statement, expecting semicolon");
      }
    }
  }
  auto ret =
      static_cast<SimpleASTNode*>(ast_builder_.AddASTNode(std::move(node)));
  return LogIfNullAndReturn(ret, "IntDeclare");

  // if (int_declare_found && node) {
  //   return static_cast<SimpleASTNode*>(
  //       ast_builder_.AddASTNode(std::move(node)));
  // }
  // VLOG(1) << "NOT IntDeclare ...";
  // return nullptr;
}

/**
 * 加法表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::Additive(TokenReader* tokens) {
  VLOG(1) << "parsing Additive...";
  SimpleASTNode* child1 = Multiplicative(tokens);
  // SimpleASTnode* node = child1;
  if (child1 != nullptr) {
    while (true) {
      const Token* ptoken = tokens->Peek();
      if (CheckTokenType(ptoken, TokenType::Plus) ||
          CheckTokenType(ptoken, TokenType::Minus)) {
        ptoken = tokens->Read();  // get the "+"
        SimpleASTNode* child2 = Multiplicative(tokens);
        // CHECK(child2)
        //     << "invalid additive expression, expecting the right part.";
        if (child2 == nullptr) {
          throw std::logic_error(
              "invalid additive expression, expecting the right part.");
        }
        std::unique_ptr<SimpleASTNode> add_node =
            std::make_unique<SimpleASTNode>(ASTNodeType::Additive,
                                            ptoken->Text());
        add_node->AddChild(child1);  // Attention, new ast node on the top to
                                     // ensure the correctness
        add_node->AddChild(child2);
        // child1 = add_node.get();
        child1 = static_cast<SimpleASTNode*>(
            ast_builder_.AddASTNode(std::move(add_node)));
        // child1 = add_node.get();
        // node = add_node.get();
      } else {
        break;
      }
    }
  }
  return LogIfNullAndReturn(child1, "Additive");
  // return child1;
}

/**
 * 乘法表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::Multiplicative(TokenReader* tokens) {
  VLOG(1) << "parsing Multiplicative...";
  SimpleASTNode* child1 = Primary(tokens);
  if (child1 != nullptr) {
    while (true) {
      const Token* ptoken = tokens->Peek();
      if (CheckTokenType(ptoken, TokenType::Star) ||
          CheckTokenType(ptoken, TokenType::Slash)) {
        ptoken = tokens->Read();  // take the "*" or "/"
        VLOG(2) << "  got " << ptoken->Text();
        SimpleASTNode* child2 = Primary(tokens);
        // CHECK(child2)
        //     << "invalid multiplicative expression, expecting the right
        //     part.";
        if (child2 == nullptr) {
          throw std::logic_error(
              "invalid multiplicative expression, expecting the right part.");
        }
        std::unique_ptr<SimpleASTNode> mul_node =
            std::make_unique<SimpleASTNode>(ASTNodeType::Multiplicative,
                                            ptoken->Text());
        mul_node->AddChild(child1);
        mul_node->AddChild(child2);
        child1 = static_cast<SimpleASTNode*>(
            ast_builder_.AddASTNode(std::move(mul_node)));

      } else {
        break;
      }
    }
  }
  // if(child1 == nullptr) {
  //   VLOG(1) << ""
  // }
  return LogIfNullAndReturn(child1, "Multiplicative");
  // return child1;
}

/**
 * 基础表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::Primary(TokenReader* tokens) {
  VLOG(1) << "parsing Primary...";
  SimpleASTNode* node = nullptr;
  // std::unique_ptr<SimpleASTNode> node;
  const Token* ptoken = tokens->Peek();
  if (CheckTokenType(ptoken, TokenType::IntLiteral)) {
    ptoken = tokens->Read();
    VLOG(2) << "  got IntLiteral: " << ptoken->Text();
    auto pri_node = absl::WrapUnique(
        new SimpleASTNode(ASTNodeType::IntLiteral, ptoken->Text()));
    node = static_cast<SimpleASTNode*>(
        ast_builder_.AddASTNode(std::move(pri_node)));
  } else if (CheckTokenType(ptoken, TokenType::Identifier)) {
    ptoken = tokens->Read();
    VLOG(2) << "  got Identifier: " << ptoken->Text();
    auto pri_node = absl::WrapUnique(
        new SimpleASTNode(ASTNodeType::Identifier, ptoken->Text()));
    node = static_cast<SimpleASTNode*>(
        ast_builder_.AddASTNode(std::move(pri_node)));
  } else if (CheckTokenType(ptoken, TokenType::LeftParen)) {
    ptoken = tokens->Read();  // eat the "("
    VLOG(2) << "  got LeftParen: " << ptoken->Text();
    node = Additive(tokens);
    // CHECK(node) << "expecting an additive expression inside parenthesis";
    if (node == nullptr) {
      throw std::logic_error(
          "expecting an additive expression inside parenthesis");
    }
    ptoken = tokens->Peek();
    // CHECK(CheckTokenType(ptoken, TokenType::RightParen))
    //     << "expecting right parenthesis";
    if (!CheckTokenType(ptoken, TokenType::RightParen)) {
      throw std::logic_error("expecting right parenthesis");
    }
    ptoken = tokens->Read();
    VLOG(2) << "  got RightParen: " << ptoken->Text();
  }
  return LogIfNullAndReturn(node, "Primary");
  // return node;
}

void SimpleParser::DumpAST(const ASTNode* node,
                           const std::string& indent) const {
  // VLOG(0) << "Dumping node :" << node;
  // VLOG(0) << "node's parent: " << node->GetParent();
  // VLOG(0) << "node's children: " << node->GetChildren().size();
  std::cout << indent
            << ASTNodeTypeToString(node->GetType()) + " " + node->GetText()
            << std::endl;
  // VLOG(0) << indent
  //         << ASTNodeTypeToString(node->GetType()) + " " + node->GetText();
  // VLOG(0) << "the dumped string as above...";
  for (auto& child : node->GetChildren()) {
    DumpAST(child, indent + "\t");
  }
}
