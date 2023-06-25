#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "AntlrHeader.h"
#include "SQLVisitor.h"
#include "SQLiteLexer.h"
#include "SQLiteParser.h"
// #include "SelectStmt.h"
#include "SQLVisitor.h"

namespace playdb {

class PlayDB {
 public:
  PlayDB() {
    region2DB["SDYT"] = "db1";  //山东烟台
    region2DB["BJHD"] = "db2";  //北京海淀
    region2DB["FJXM"] = "db3";  //福建厦门
    region2DB["SZLG"] = "db4";  //深圳龙岗
  }

  std::string getDBName(std::string sql) {
    // // lexical analysis
    // ANTLRInputStream input(script);
    // lexer_.reset(new PlayScriptLexer(&input));
    // CommonTokenStream tokens(lexer_.get());

    // // syntax analysis
    // parser_.reset(new PlayScriptParser(&tokens));
    // at_->ast = parser_->prog();

    // lexical analysis
    ANTLRInputStream input(sql);
    std::shared_ptr<SQLiteLexer> lexer = std::make_shared<SQLiteLexer>(&input);
    CommonTokenStream tokens(lexer.get());

    // syntax analysis
    std::shared_ptr<SQLiteParser> parser =
        std::make_shared<SQLiteParser>(&tokens);
    ParseTree* tree = parser->sql_stmt();

    // 以lisp格式打印AST
    std::cout << tree->toStringTree() << std::endl;

    // 获得select语句的要素
    std::shared_ptr<SQLiteVisitor> visitor = std::make_shared<SQLVisitor>();
    // SQLiteParser::Select
    SelectStmt* select = visitor->visit(tree).as<SelectStmt*>();

    std::string dbName;
    if (select->tableName == "orders") {
      if (!select->whereExprs.empty()) {
        for (WhereExpr* expr : select->whereExprs) {
          // 根据cust_id或order_id来确定库的名称
          if (expr->columnName == "cust_id" || expr->columnName == "order_id") {
            // 取编号的前4位，即区域编码
            // std::cout << "whereExpr's col name: " << expr->columnName
            //           << ", value: " << expr->value << std::endl;
            // db name starts with "'", so substring index starts from 1
            std::string region = expr->value.substr(1, 4);
            // std::cout << "found region: " << region << std::endl;
            dbName = region2DB[region];
            break;
          }
        }
      }
    }

    // if (dbName.empty()) {
    //   assert(false && "Error: no database name found!");
    // }

    return dbName;
  }

 private:
  std::unordered_map<std::string, std::string> region2DB;
};
}  // namespace playdb
