#include "simple_script.h"

#include <iostream>

#include "ast_node_base.h"
#include "map_util.h"
#include "simple_parser.h"
// #include "simp"

bool SimpleScript::verbose_ = false;

int SimpleScript::Evaluate(const ASTNode* node, const std::string& indent) {
  int result;
  if (verbose_) {
    std::cout << indent + "Calculating: " << node->GetType() << std::endl;
  }

  ASTNode* child1 = nullptr;
  ASTNode* child2 = nullptr;
  int value1, value2;
  std::string var_name;

  switch (node->GetType()) {
    case ASTNodeType::Program:
      for (ASTNode* child : node->GetChildren()) {
        result = Evaluate(child, indent);
      }
      break;
    case ASTNodeType::Additive:
      child1 = node->GetChildren()[0];
      value1 = Evaluate(child1, indent + "\t");
      child2 = node->GetChildren()[1];
      value2 = Evaluate(child2, indent + "\t");
      if (node->GetText() == "+") {
        result = value1 + value2;
      } else {
        result = value1 - value2;
      }
      break;
    case ASTNodeType::Multiplicative:
      child1 = node->GetChildren()[0];
      value1 = Evaluate(child1, indent + "\t");
      child2 = node->GetChildren()[1];
      value2 = Evaluate(child2, indent + "\t");
      if (node->GetText() == "*") {
        result = value1 * value2;
      } else {
        result = value1 / value2;
      }
      break;
    case ASTNodeType::IntLiteral:
      result = std::stoi(node->GetText());
      break;
    case ASTNodeType::Identifier:
      var_name = node->GetText();
      if (ContainsKey(variables_, var_name)) {
        // The variables_[var_name] may not be initialized, we do not deal with
        // this case.
        result = variables_[var_name];
      } else {
        throw std::logic_error("unknown variable: " + var_name);
      }
      break;
    case ASTNodeType::AssignmentStmt:
      var_name = node->GetText();
      if (!ContainsKey(variables_, var_name)) {
        throw std::logic_error("unknown variable: " + var_name);
      }  // continue to the next case
    case ASTNodeType::IntDeclaration:
      var_name = node->GetText();
      int var_value;
      if (node->GetChildren().size() > 0) {
        ASTNode* child = node->GetChildren()[0];
        result = Evaluate(child, indent + "\t");
      }
      variables_[var_name] = result;
      break;
    default:
      // None
      throw std::logic_error("unknown AST node type!");
  }
  if (verbose_) {
    std::cout << indent + "Result: " << result << std::endl;
  } else if (indent == "") {  // the top output
    if (node->GetType() == ASTNodeType::IntDeclaration ||
        node->GetType() == ASTNodeType::AssignmentStmt) {
      std::cout << node->GetText() + ": " << result << std::endl;
    } else if (node->GetType() != ASTNodeType::Program) {
      std::cout << result << std::endl;
    }
  }
  return result;
}
