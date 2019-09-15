#ifndef AST_NODE_BASE_H_
#define AST_NODE_BASE_H_

#include <string>
#include <vector>

#include "ast_node_type.h"

/**
 * AST的节点。
 * 属性包括AST的类型、文本值、下级子节点和父节点
 */
class ASTNode {
 public:
  //父节点
  virtual const ASTNode* GetParent() const = 0;

  //子节点
  virtual const std::vector<ASTNode*> GetChildren() const = 0;

  // AST类型
  virtual ASTNodeType GetType() const = 0;

  //文本值
  virtual const std::string& GetText() const = 0;
};

#endif  // AST_NODE_BASE_H_
