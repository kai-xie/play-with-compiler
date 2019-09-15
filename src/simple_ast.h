#ifndef SIMPLE_AST_H_
#define SIMPLE_AST_H_

#include <memory>
// #include <string>
#include <vector>

class ASTNode;

// A class holds all the ASTNode in an abstract syntax tree.
class SimpleAST {
 public:
  class Builder {
   public:
    explicit Builder() : builder_ast_nodes_(), first_added_node_(nullptr) {}

    std::unique_ptr<SimpleAST> Build(ASTNode* root_ast_node = nullptr);

    ASTNode* AddASTNode(std::unique_ptr<ASTNode> ast_node);

   private:
    std::vector<std::unique_ptr<ASTNode>> builder_ast_nodes_;
    ASTNode* first_added_node_;
  };

  const std::vector<const ASTNode*> ASTNodes() const;
  std::vector<ASTNode*> ASTNodes();

  const ASTNode* root_node() const { return root_node_; }
  ASTNode* root_node() { return root_node_; }

 private:
  SimpleAST(std::vector<std::unique_ptr<ASTNode>>* ast_nodes,
            ASTNode* root_ast_node);

  // Stores all the ast node.
  std::vector<std::unique_ptr<ASTNode>> ast_nodes_;

  // The root in the AST.
  ASTNode* root_node_;
};

// // std::ostream& operator<<(std::ostream& s, std::nullptr_t) {
// std::ostream& operator<<(std::ostream& s, std::nullptr_t& p) {
//   return s << std::string("nullptr");
// }

#endif  // SIMPLE_AST_H_
