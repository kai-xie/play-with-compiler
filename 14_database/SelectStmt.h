#pragma once

#include <vector>

#include "WhereExpr.h"

namespace playdb {
/**
 * 保存select查询语句的概要信息
 */
class SelectStmt {
 public:
  std::string tableName;
  std::vector<WhereExpr*> whereExprs;

  SelectStmt(const std::string& table_name,
             const std::vector<WhereExpr*>& where_exprs)
      : tableName(table_name), whereExprs(where_exprs) {}
};

}  // namespace playdb
