#pragma once

#include <vector>

#include "AntlrHeader.h"
#include "SQLiteBaseVisitor.h"
#include "SQLiteParser.h"
#include "SelectStmt.h"
#include "WhereExpr.h"

namespace playdb {
class SQLVisitor : public SQLiteBaseVisitor {
 public:
  virtual antlrcpp::Any visitSql_stmt(
      SQLiteParser::Sql_stmtContext* ctx) override {
    antlrcpp::Any result;
    if (auto* fac_sel_stmt = ctx->factored_select_stmt()) {
      if (fac_sel_stmt->select_core().size() > 0) {
        result = visitSelect_core(fac_sel_stmt->select_core(0));
      }
    }
    return result;
  }

  virtual antlrcpp::Any visitSelect_core(
      SQLiteParser::Select_coreContext* ctx) override {
    antlrcpp::Any result;
    std::string tableName;
    std::vector<WhereExpr*> whereExprs;

    if (ctx->K_FROM()) {
      if (ctx->table_or_subquery().size() > 0) {
        tableName =
            visitTable_or_subquery(ctx->table_or_subquery(0)).as<std::string>();
      }
    }
    if (ctx->K_WHERE()) {
      if (ctx->expr().size() > 0) {
        whereExprs = visitExpr(ctx->expr(0)).as<std::vector<WhereExpr*>>();
      }
    }

    result = new SelectStmt(tableName, whereExprs);
    return result;
  }

  virtual antlrcpp::Any visitAny_name(
      SQLiteParser::Any_nameContext* ctx) override {
    antlrcpp::Any result;
    std::cout << ctx->toString() << std::endl;
    if (ctx->IDENTIFIER()) {
      result = ctx->IDENTIFIER()->getText();
    }
    return result;
  }

  virtual antlrcpp::Any visitColumn_name(
      SQLiteParser::Column_nameContext* ctx) override {
    antlrcpp::Any result;
    if (ctx->any_name()) {
      result = visitAny_name(ctx->any_name());
    }
    return result;
  }

  virtual antlrcpp::Any visitTable_name(
      SQLiteParser::Table_nameContext* ctx) override {
    antlrcpp::Any result;
    if (ctx->any_name()) {
      result = visitAny_name(ctx->any_name());
    }
    return result;
  }

  virtual antlrcpp::Any visitFactored_select_stmt(
      SQLiteParser::Factored_select_stmtContext* ctx) override {
    antlrcpp::Any result;
    return result;
  }

  virtual antlrcpp::Any visitResult_column(
      SQLiteParser::Result_columnContext* ctx) override {
    antlrcpp::Any result;
    return result;
  }

  virtual antlrcpp::Any visitTable_or_subquery(
      SQLiteParser::Table_or_subqueryContext* ctx) override {
    antlrcpp::Any result;
    if (ctx->table_name()) {
      result = visitTable_name(ctx->table_name());
    }
    return result;
  }

  virtual antlrcpp::Any visitExpr(SQLiteParser::ExprContext* ctx) override {
    antlrcpp::Any result;
    if (ctx->K_AND()) {
      antlrcpp::Any left = visitExpr(ctx->expr(0));
      antlrcpp::Any right = visitExpr(ctx->expr(1));
      std::vector<WhereExpr*> list;
      addWhereExpr(list, left);
      addWhereExpr(list, right);
      result = list;
    } else if (ctx->ASSIGN()) {
      std::string left = visitExpr(ctx->expr(0)).as<std::string>();
      std::string right = visitExpr(ctx->expr(1)).as<std::string>();
      result = new WhereExpr(left, "=", right);
    } else if (ctx->literal_value()) {
      result = visitLiteral_value(ctx->literal_value());
    } else if (ctx->column_name()) {
      result = visitColumn_name(ctx->column_name());
    }
    return result;
  }

  virtual antlrcpp::Any visitLiteral_value(
      SQLiteParser::Literal_valueContext* ctx) override {
    antlrcpp::Any result;
    if (ctx->STRING_LITERAL()) {
      result = ctx->STRING_LITERAL()->getText();
    }
    return result;
  }

 private:
  void addWhereExpr(std::vector<WhereExpr*>& list, antlrcpp::Any obj) {
    // TODO
    if (obj.is<WhereExpr*>()) {
      list.emplace_back(obj.as<WhereExpr*>());
    } else if (obj.is<std::vector<WhereExpr*>>()) {
      std::vector<WhereExpr*> wheres = obj.as<std::vector<WhereExpr*>>();
      list.insert(list.end(), wheres.begin(), wheres.end());
    }
  }
};
}  // namespace playdb
