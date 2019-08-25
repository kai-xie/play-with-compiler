#include "ast_node_type.h"

#include <string>

std::string ASTNodeTypeToString(ASTNodeType ast_node_type) {
  switch (ast_node_type) {
#define CASE_ENUM_TYPE_STRING(enum_type, enum_name) \
  case ASTNodeType::enum_type:                      \
    return enum_name;
    ASTNODE_TYPE_LIST(CASE_ENUM_TYPE_STRING)
#undef CASE_ENUM_TYPE_STRING
  }
}
