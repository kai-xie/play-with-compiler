#include <vector>
// #include

#include "absl/container/flat_hash_set.h"
#include "glog/logging.h"

#include "ast_node_base.h"
#include "ast_node_type.h"
#include "map_util.h"

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

template <typename T>
inline T* AddNodeToBuilder(SimpleAST::Builder& builder,
                           std::unique_ptr<T> node) {
  T* ret = node.get();
  builer.AddASTNode(node);
  return ret;
}

/**
 * AST的根节点，解析的入口。
 * @return
 * @throws Exception
 */
SimpleASTNode* SimpleParser::Prog(TokenReaderBase tokens) {
  std::unique_ptr<SimpleASTNode> node =
      std::make_unique<SimpleASTNode>(ASTNodeType::Program, "pwc");
  while (tokens.peek() != nullptr) {
    std::unique_ptr<SimpleASTNode> child = IntDeclare(tokens);
    if (nullptr == child) {
      child = ExpressionStatement(tokens);
    }
    if (nullptr == child) {
      child = AssignmentStatement(tokens);
    }
    CHECK(child.get()) << "unknown statement";
    if (nullptr != child) {
      node->AddChild(child);
    }
  }
  // auto ret = node.get();
  // ast_builder_.AddASTNode(node);
  // return ret;
  return AddNodeToBuilder(ast_builder_, node);
};

/**
 * 表达式语句，即表达式后面跟个分号。
 * @return
 * @throws Exception
 */
SimpleASTNode* ExpressionStatement(TokenReaderBase tokens) {
  int pos = tokens.GetPosition();
  SimpleASTNode* node = Additive(tokens);
  if (node != nullptr) {
    TokenBase* token = tokens.peek();
    if (token != nullptr && token.Type() == TokenType::SemiColon) {
      tokens.reade();
    } else {
      node = nullptr;
      tokens.SetPosition(pos);
    }
  }
  return node;
}

    /**
     * 赋值语句，如age = 10*2;
     * @return
     * @throws Exception
     */









/**
 * 一个简单的AST节点。
 * 属性包括：类型、文本值、父节点、子节点。
 */
class SimpleASTNode : public ASTNodeBase {
 public:
  SimpleASTNode(ASTNodeType node_type, const std::string& text)
      : node_type_(node_type), text_(text) {}

  virtual const ASTNodeBase* GetParent() const override { return parent_; }

  //子节点
  virtual const std::vector<ASTNodeBase*> GetChildren() const override {
    return children_;
  }

  // AST类型
  virtual ASTNodeType GetType() const override { return node_type_; }

  //文本值
  virtual const std::string GetText() const override { return text_; }

  void AddChild(SimpleASTNode* child) {
    if (!ContainsKey(children_set_, child)) {
      children_.emplace_back(child);
      children_set_.insert(child);
      child->parent_ = this;
    }
  }

 private:
  SimpleASTNode* parent_;

  std::vector<ASTNodeBase*> children_;
  absl::flat_hash_set<const ASTNodeBase*> children_set_;
  // std::set<ASTNodeBase*> children_set_;

  ASTNodeType node_type_;
  std::string text_;
};

// private
// class SimpleASTNode implements ASTNode {
//   SimpleASTNode parent = null;
//   List<ASTNode> children = new ArrayList<ASTNode>();
//   List<ASTNode> readonlyChildren = Collections.unmodifiableList(children);
//   ASTNodeType nodeType = null;
//   String text = null;

//  public
//   SimpleASTNode(ASTNodeType nodeType, String text) {
//     this.nodeType = nodeType;
//     this.text = text;
//   }

//   @Override public ASTNode getParent() {
//     return parent;
//   }

//   @Override public List<ASTNode> getChildren() {
//     return readonlyChildren;
//   }

//   @Override public ASTNodeType getType() {
//     return nodeType;
//   }

//   @Override public String getText() {
//     return text;
//   }

//  public
//   void addChild(SimpleASTNode child) {
//     children.add(child);
//     child.parent = this;
//   }

//     }
