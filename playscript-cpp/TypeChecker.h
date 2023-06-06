#pragma once

#include "AnnotatedTree.h"
#include "AntlrPlayScript.h"
#include "PlayScriptBaseListener.h"
#include "PlayScriptParser.h"
#include "PrimitiveType.h"

namespace play {
/**
 * 类型检查。
 * 主要检查:
 * 1.赋值表达式；
 * 2.变量初始化；
 * 3.表达式里的一些运算，比如加减乘除，是否类型匹配；
 * 4.返回值的类型；
 *
 */
class TypeChecker : public PlayScriptBaseListener {
 private:
  std::shared_ptr<AnnotatedTree> at;

 public:
  TypeChecker(std::shared_ptr<AnnotatedTree> an_tree) : at(an_tree) {}

  virtual void exitVariableDeclarator(
      PlayScriptParser::VariableDeclaratorContext* ctx) override {
    if (ctx->variableInitializer()) {
      Variable* variable = dynamic_cast<Variable*>(
          at->symbolOfNode[ctx->variableDeclaratorId()]);
      Type* type1 = variable->type;
      Type* type2 = at->typeOfNode[ctx->variableDeclaratorId()];
      checkAssign(type1, type2, ctx, ctx->variableDeclaratorId(),
                  ctx->variableInitializer());
    }
  }

  virtual void exitExpression(
      PlayScriptParser::ExpressionContext* ctx) override {
    if (ctx->bop && ctx->expression().size() >= 2) {
      Type* type1 = at->typeOfNode[ctx->expression(0)];
      Type* type2 = at->typeOfNode[ctx->expression(1)];

      switch (ctx->bop->getType()) {
        case PlayScriptParser::ADD:
          //字符串能够跟任何对象做 + 运算
          if (!type1->isType(PrimitiveType::String()) &&
              !type2->isType(PrimitiveType::String())) {
            checkNumericOperand(type1, ctx, ctx->expression(0));
            checkNumericOperand(type2, ctx, ctx->expression(1));
          }
          break;
        case PlayScriptParser::SUB:
        case PlayScriptParser::MUL:
        case PlayScriptParser::DIV:
        case PlayScriptParser::LE:
        case PlayScriptParser::LT:
        case PlayScriptParser::GE:
        case PlayScriptParser::GT:
          checkNumericOperand(type1, ctx, ctx->expression(0));
          checkNumericOperand(type1, ctx, ctx->expression(1));
          break;
        case PlayScriptParser::EQUAL:
        case PlayScriptParser::NOTEQUAL:

          break;

        case PlayScriptParser::AND:
        case PlayScriptParser::OR:
          checkBooleanOperand(type1, ctx, ctx->expression(0));
          checkBooleanOperand(type2, ctx, ctx->expression(1));
          break;

        case PlayScriptParser::ASSIGN:
          checkAssign(type1, type2, ctx, ctx->expression(0),
                      ctx->expression(1));
          break;

        case PlayScriptParser::ADD_ASSIGN:
        case PlayScriptParser::SUB_ASSIGN:
        case PlayScriptParser::MUL_ASSIGN:
        case PlayScriptParser::DIV_ASSIGN:
        case PlayScriptParser::AND_ASSIGN:
        case PlayScriptParser::OR_ASSIGN:
        case PlayScriptParser::XOR_ASSIGN:
        case PlayScriptParser::MOD_ASSIGN:
        case PlayScriptParser::LSHIFT_ASSIGN:
        case PlayScriptParser::RSHIFT_ASSIGN:
        case PlayScriptParser::URSHIFT_ASSIGN:
          if (PrimitiveType::isNumeric(type2)) {
            if (!checkNumericAssign(type2, type1)) {
              at->log("can not assign " + ctx->expression(1)->getText() +
                          " of type " + type2->toString() + " to " +
                          ctx->expression(0)->getText() + " of type " +
                          type1->toString(),
                      ctx);
            }
          } else {
            at->log("operand + " + ctx->expression(1)->getText() +
                        " should be numeric。",
                    ctx);
          }
          break;
      }
    }
  }

 private:
  /**
   * 看一个类型能否赋值成另一个类型，比如：
   * (1) 整型可以转成浮点型；
   * (2) 子类的对象可以赋给父类;
   * (3) 函数赋值，要求签名是一致的。
   * @param from
   * @param to
   * @return
   */
  bool checkNumericAssign(Type* from, Type* to) {
    bool canAssign = false;
    if (to->isType(PrimitiveType::Double())) {
      canAssign = PrimitiveType::isNumeric(from);
    } else if (to->isType(PrimitiveType::Float())) {
      canAssign = (from->isType(PrimitiveType::Byte()) ||
                   from->isType(PrimitiveType::Short()) ||
                   from->isType(PrimitiveType::Integer()) ||
                   from->isType(PrimitiveType::Long()) ||
                   from->isType(PrimitiveType::Float()));
    } else if (to->isType(PrimitiveType::Long())) {
      canAssign = (from->isType(PrimitiveType::Byte()) ||
                   from->isType(PrimitiveType::Short()) ||
                   from->isType(PrimitiveType::Integer()) ||
                   from->isType(PrimitiveType::Long()));
    } else if (to->isType(PrimitiveType::Integer())) {
      canAssign = (from->isType(PrimitiveType::Byte()) ||
                   from->isType(PrimitiveType::Short()) ||
                   from->isType(PrimitiveType::Integer()));
    } else if (to->isType(PrimitiveType::Short())) {
      canAssign = (from->isType(PrimitiveType::Byte()) ||
                   from->isType(PrimitiveType::Short()));
    } else if (to->isType(PrimitiveType::Byte())) {
      canAssign = from->isType(PrimitiveType::Byte());
    }

    return canAssign;
  }

  /**
   * 检查是否能做赋值操作
   * @param type1
   * @param type2
   * @param ctx
   * @param operand1
   * @param operand2
   */
  void checkAssign(Type* type1, Type* type2, ParserRuleContext* ctx,
                   ParserRuleContext* operand1, ParserRuleContext* operand2) {
    if (PrimitiveType::isNumeric(type2)) {
      if (!checkNumericAssign(type2, type1)) {
        at->log("can not assign " + operand2->getText() + " of type " +
                    type2->toString() + " to " + operand1->getText() +
                    " of type " + type1->toString(),
                ctx);
      }
    } else if (antlrcpp::is<Class*>(type2)) {
      // TODO 检查类的兼容性
    } else if (antlrcpp::is<Function*>(type2)) {
      // TODO 检查函数的兼容性
    }
  }

  //    /**
  //     * 检查加法的操作数。允许数值和字符串
  //     *
  //     * @param type
  //     * @param exp
  //     * @param operand
  //     */
  //    private void checkAddOperand(Type type, ExpressionContext exp,
  //    ExpressionContext operand) {
  //        if (!(PrimitiveType.isNumeric(type) || type ==
  //        PrimitiveType.String)) {
  //            at.log("operand for add should be numeric or string: " +
  //            operand.getText(), exp);
  //        }
  //    }

  /**
   * 检查类型是不是数值型的。
   *
   * @param type
   * @param exp
   * @param operand
   */
  void checkNumericOperand(Type* type, PlayScriptParser::ExpressionContext* exp,
                           PlayScriptParser::ExpressionContext* operand) {
    if (!(PrimitiveType::isNumeric(type))) {
      at->log("operand for arithmetic operation should be numeric : " +
                  operand->getText(),
              exp);
    }
  }

  /**
   * 检查类型是不是Boolean型的
   *
   * @param type
   * @param exp
   * @param operand
   */
  void checkBooleanOperand(Type* type, PlayScriptParser::ExpressionContext* exp,
                           PlayScriptParser::ExpressionContext* operand) {
    if (!(type->isType(PrimitiveType::Boolean()))) {
      at->log("operand for logical operation should be boolean : " +
                  operand->getText(),
              exp);
    }
  }
};

}  // namespace play
