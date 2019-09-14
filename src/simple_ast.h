#ifndef SIMPLE_AST_H_
#define SIMPLE_AST_H_

#include <memory>
// #include <string>
#include <vector>

class ASTNodeBase;

// A class holds all the ASTNode in an abstract syntax tree.
class SimpleAST {
 public:
  class Builder {
   public:
    explicit Builder() : builder_ast_nodes_(), first_added_node_(nullptr) {}

    std::unique_ptr<SimpleAST> Build(ASTNodeBase* root_ast_node = nullptr);

    void AddASTNode(std::unique_ptr<ASTNodeBase> ast_node);

   private:
    std::vector<std::unique_ptr<ASTNodeBase>> builder_ast_nodes_;
    ASTNodeBase* first_added_node_;
  };

 private:
  SimpleAST(std::vector<std::unique_ptr<ASTNodeBase>>* ast_nodes,
            ASTNodeBase* root_ast_node);

  // Stores all the ast node.
  std::vector<std::unique_ptr<ASTNodeBase>> ast_nodes_;

  // The root in the AST.
  ASTNodeBase* root_;
};

// // std::ostream& operator<<(std::ostream& s, std::nullptr_t) {
// std::ostream& operator<<(std::ostream& s, std::nullptr_t& p) {
//   return s << std::string("nullptr");
// }

#endif  // SIMPLE_AST_H_
