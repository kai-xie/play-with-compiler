#include "simple_calculator.h"

#include <numeric>

#include "ast_node_type.h"
#include "map_util.h"
#include "simple_ast.h"
#include "simple_ast_node.h"
#include "simple_lexer.h"
#include "simple_token_reader.h"
#include "token_base.h"
#include "token_type.h"

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
 * 执行脚本，并打印输出AST和求值过程。
 * @param script
 */
void SimpleCalculator::Evaluate(const std::string& script) {
  try {
    std::unique_ptr<SimpleAST> tree = Parse(script);
    DumpAST(tree->root_node(), "");
    Evaluate(tree->root_node(), "");
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return;
  }
}

/**
 * 解析脚本，并返回根节点
 * @param code
 * @return
 * @throws Exception
 */
std::unique_ptr<SimpleAST> SimpleCalculator::Parse(const std::string& code) {
  std::unique_ptr<SimpleLexer> lexer = absl::WrapUnique(new SimpleLexer());
  auto simple_token_reader = lexer->Tokenize(code);
  // TokenReader* tokens =
  // dynamic_cast<TokenReader*>(simple_token_reader.get());
  TokenReader* tokens = simple_token_reader.get();
  ASTNode* root_node = static_cast<SimpleASTNode*>(Prog(tokens));
  return ast_builder_.Build(root_node);
}

/**
 * 对某个AST节点求值，并打印求值过程。
 * @param node
 * @param indent  打印输出时的缩进量，用tab控制
 * @return
 */
int SimpleCalculator::Evaluate(ASTNode* node, const std::string& indent) {
  int result = 0;
  std::cout << indent + "Calculating: " + ASTNodeTypeToString(node->GetType())
            << std::endl;
  ASTNode* child1 = nullptr;
  ASTNode* child2 = nullptr;

  switch (node->GetType()) {
    case ASTNodeType::Program:
      for (auto child : node->GetChildren()) {
        result = Evaluate(child, indent + "\t");
      }
      break;
    case ASTNodeType::Additive: {
      child1 = node->GetChildren()[0];
      int value1 = Evaluate(child1, indent + "\t");
      child2 = node->GetChildren()[1];
      int value2 = Evaluate(child2, indent + "\t");
      if (node->GetText() == "+") {
        result = value1 + value2;
      } else {
        result = value1 - value2;
      }
      break;
    }
    case ASTNodeType::Multiplicative: {
      child1 = node->GetChildren()[0];
      int value1 = Evaluate(child1, indent + "\t");
      child2 = node->GetChildren()[1];
      int value2 = Evaluate(child2, indent + "\t");
      if (node->GetText() == "*") {
        result = value1 * value2;
      } else {
        result = value1 / value2;
      }
      break;
    }
    case ASTNodeType::IntLiteral:
      result = std::stoi(node->GetText());
      break;
    default:
      break;
  }
  std::cout << indent + "Result: " << result << std::endl;
  return result;
}

/**
 * 语法解析：根节点
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleCalculator::Prog(TokenReader* tokens) {
  std::unique_ptr<SimpleASTNode> node =
      std::make_unique<SimpleASTNode>(ASTNodeType::Program, "Calculator");

  SimpleASTNode* child = IntDeclare(tokens);
  if (nullptr == child) {
    child = Additive(tokens);
  }
  if (child != nullptr) {
    node->AddChild(child);
  } else {
    // child = IntDeclare(tokens)
    throw std::logic_error("unknown statement");
  }
  SimpleASTNode* ret =
      static_cast<SimpleASTNode*>(ast_builder_.AddASTNode(std::move(node)));
  return ret;
}

/**
 * 整型变量声明语句，如：
 * int a;
 * int b = 2*3;
 *
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleCalculator::IntDeclare(TokenReader* tokens) {
  VLOG(1) << "parsing IntDeclare...";
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
        if (child == nullptr) {
          throw std::logic_error(
              "invalide variable initialization, expecting an expression");
        }
        node->AddChild(child);
      }
    } else {
      throw std::logic_error("variable name expected");
    }

    if (node) {
      ptoken = tokens->Peek();
      if (CheckTokenType(ptoken, TokenType::SemiColon)) {
        tokens->Read();
      } else {
        throw std::logic_error("invalid statement, expecting semicolon");
      }
    }
  }
  auto ret =
      static_cast<SimpleASTNode*>(ast_builder_.AddASTNode(std::move(node)));
  // return LogIfNullAndReturn(ret, "IntDeclare");
  return ret;
}

/**
 * 语法解析：加法表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleCalculator::Additive(TokenReader* tokens) {
  VLOG(1) << "parsing Additive...";
  SimpleASTNode* child1 = Multiplicative(tokens);
  // SimpleASTnode* node = child1;
  const Token* ptoken = tokens->Peek();
  if (child1 != nullptr && ptoken != nullptr) {
    if (CheckTokenType(ptoken, TokenType::Plus) ||
        CheckTokenType(ptoken, TokenType::Minus)) {
      ptoken = tokens->Read();  // get the "+" or "-"
      SimpleASTNode* child2 = Additive(tokens);
      // CHECK(child2)
      //     << "invalid additive expression, expecting the right part.";
      if (child2 == nullptr) {
        throw std::logic_error(
            "invalid additive expression, expecting the right part.");
      }
      std::unique_ptr<SimpleASTNode> add_node = std::make_unique<SimpleASTNode>(
          ASTNodeType::Additive, ptoken->Text());
      add_node->AddChild(child1);  // Attention, new ast node on the top to
                                   // ensure the correctness
      add_node->AddChild(child2);
      child1 = static_cast<SimpleASTNode*>(
          ast_builder_.AddASTNode(std::move(add_node)));
    }
  }
  return LogIfNullAndReturn(child1, "Additive");
}

/**
 * 语法解析：乘法表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleCalculator::Multiplicative(TokenReader* tokens) {
  VLOG(1) << "parsing Multiplicative...";
  SimpleASTNode* child1 = Primary(tokens);
  if (child1 != nullptr) {
    const Token* ptoken = tokens->Peek();
    if (CheckTokenType(ptoken, TokenType::Star) ||
        CheckTokenType(ptoken, TokenType::Slash)) {
      ptoken = tokens->Read();  // take the "*" or "/"
      VLOG(2) << "  got " << ptoken->Text();
      SimpleASTNode* child2 = Primary(tokens);
      if (child2 == nullptr) {
        throw std::logic_error(
            "invalid multiplicative expression, expecting the right part.");
      }
      std::unique_ptr<SimpleASTNode> mul_node = std::make_unique<SimpleASTNode>(
          ASTNodeType::Multiplicative, ptoken->Text());
      mul_node->AddChild(child1);
      mul_node->AddChild(child2);
      child1 = static_cast<SimpleASTNode*>(
          ast_builder_.AddASTNode(std::move(mul_node)));
    }
  }
  return LogIfNullAndReturn(child1, "Multiplicative");
}

/**
 * 语法解析：基础表达式
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleCalculator::Primary(TokenReader* tokens) {
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
    ptoken = tokens->Read();  // eat the ")"
    VLOG(2) << "  got RightParen: " << ptoken->Text();
  }
  return LogIfNullAndReturn(node, "Primary");
}

void SimpleCalculator::DumpAST(const ASTNode* node,
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
