#include <vector>
#include <string>

#include "ast_node_type.h"

/**
 * AST的节点。
 * 属性包括AST的类型、文本值、下级子节点和父节点
 */
class ASTNodeBase{
    //父节点
    virtual ASTNodeBase GetParent() = 0;

    //子节点
    std::vector<ASTNodeBase> GetChildren() =0 ;

    //AST类型
    ASTNodeType GetType() = 0;

    //文本值
    std::string GetText() = 0;
};
