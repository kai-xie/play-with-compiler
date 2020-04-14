#ifndef SIMPLE_AST_NODE_H_
#define SIMPLE_AST_NODE_H_

#include "absl/container/flat_hash_set.h"
#include "glog/logging.h"

#include "ast_node_base.h"

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

  virtual ASTNode* GetChildren(int child_idx) {
    CHECK_GE(child_idx, 0);
    CHECK_LT(child_idx, children_.size());

    return children_[child_idx];
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

#endif  // SIMPLE_AST_NODE_H_
