#pragma once

#include <string>

namespace playdb {
/**
 * 保存where条件的概要信息
 */
class WhereExpr {
 public:
  std::string columnName;
  std::string op = "=";
  std::string value;

  WhereExpr(const std::string& col_name, const std::string& op,
            const std::string& val)
      : columnName(col_name), op(op), value(val) {}
};

}  // namespace playdb
