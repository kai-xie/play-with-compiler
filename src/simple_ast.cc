#include "simple_ast.h"

#include "absl/memory/memory.h"
#include "glog/logging.h"

void SimpleAST::Builder::AddASTNode(std::unique_ptr<ASTNodeBase> ast_node) {
  if (builder_ast_nodes_.empty()) {
    first_added_node_ = ast_node.get();
  }
  builder_ast_nodes_.emplace_back(std::move(ast_node));
}

std::unique_ptr<SimpleAST> SimpleAST::Builder::Build(
    ASTNodeBase* root_ast_node) {
  ASTNodeBase* root = root_ast_node ? root_ast_node : first_added_node_;
  CHECK_NE(root, nullptr);
  return absl::WarpUnique(new SimpleAST(&builder_ast_nodes_, root));
}

SimpleAST::SimpleAST(std::unique_ptr<ASTNodeBase>* ast_nodes,
                     ASTNodeBase* root_ast_node)
    : root_(root_ast_node) {
  bool root_found;
  for (auto& node : *ast_nodes) {
    root_found |= node.get() == root_;
    ast_nodes_.emplace_back(std::move(node));
  }
  CHECK(root_found) << "Error: root is not found in the AST.";
}
