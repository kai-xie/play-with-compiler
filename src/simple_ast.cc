#include "simple_ast.h"

#include "absl/memory/memory.h"
#include "glog/logging.h"

#include "ast_node_base.h"
// #include "util.h"

// std::ostream& operator<<(std::ostream& s, std::nullptr_t) {
//   return s << static_cast<void*>(nullptr);
// }
// auto my_nullptr = (void*)(nullptr);

ASTNode* SimpleAST::Builder::AddASTNode(std::unique_ptr<ASTNode> ast_node) {
  if (builder_ast_nodes_.empty()) {
    first_added_node_ = ast_node.get();
  }
  builder_ast_nodes_.emplace_back(std::move(ast_node));
  return builder_ast_nodes_.back().get();
}

std::unique_ptr<SimpleAST> SimpleAST::Builder::Build(
    ASTNode* root_ast_node) {
  ASTNode* root = root_ast_node ? root_ast_node : first_added_node_;
  // CHECK_NE((void*) (0), root);
  // CHECK_NE(nullptr, root);
  // CHECK_NE(my_nullptr, root);
  CHECK(root) << "ast root should not be valid";
  return absl::WrapUnique(new SimpleAST(&builder_ast_nodes_, root));
}

SimpleAST::SimpleAST(std::vector<std::unique_ptr<ASTNode>>* ast_nodes,
                     ASTNode* root_ast_node)
    : root_node_(root_ast_node) {
  bool root_found = false;
  for (auto& node : *ast_nodes) {
    root_found |= node.get() == root_node_;
    ast_nodes_.emplace_back(std::move(node));
  }
  CHECK(root_found) << "Error: root is not found in the AST.";
}

std::vector<ASTNode*> SimpleAST::ASTNodes() {
  std::vector<ASTNode*> ret;
  for (int i = 0; i< ast_nodes_.size(); ++i) {
    ret.emplace_back(ast_nodes_[i].get());
  }
  return std::move(ret);
};

const std::vector<const ASTNode*> SimpleAST::ASTNodes() const {
  std::vector<const ASTNode*> ret;
  for (int i = 0; i< ast_nodes_.size(); ++i) {
    ret.emplace_back(ast_nodes_[i].get());
  }
  return std::move(ret);
};
