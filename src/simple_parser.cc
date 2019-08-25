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

class SimpleParser {
 public:
  // ASTNode Parse(std::string& script){}
};

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

int main(int argc, char* argv[]) {
  CHECK(true);
  VLOG(0) << "glog enbaled!";
  return 0;
}
