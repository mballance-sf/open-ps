/*
 * Expr2PSIVisitor.h
 *
 *  Created on: Apr 20, 2018
 *      Author: ballance
 */
#pragma once
#include "ExprBaseVisitor.h"
#include "IItemFactory.h"

using namespace psi_api;

class Expr2PSIVisitor : public virtual ExprBaseVisitor {
public:
	Expr2PSIVisitor(IItemFactory *factory, IBaseItem *scope);

	virtual ~Expr2PSIVisitor();

	virtual antlrcpp::Any visitEntry(ExprParser::EntryContext *ctx) override;

	  virtual antlrcpp::Any visitOpen_range_list(ExprParser::Open_range_listContext *ctx) override;

	  virtual antlrcpp::Any visitOpen_range_value(ExprParser::Open_range_valueContext *ctx) override;

	  virtual antlrcpp::Any visitExpression(ExprParser::ExpressionContext *ctx) override;

	  virtual antlrcpp::Any visitConditional_expr(ExprParser::Conditional_exprContext *ctx) override;

	  virtual antlrcpp::Any visitStatic_ref_path(ExprParser::Static_ref_pathContext *ctx) override;

	  virtual antlrcpp::Any visitVariable_ref_path(ExprParser::Variable_ref_pathContext *ctx) override;

	  virtual antlrcpp::Any visitBool_literal(ExprParser::Bool_literalContext *ctx) override;

	  virtual antlrcpp::Any visitNumber(ExprParser::NumberContext *ctx) override;

	  virtual antlrcpp::Any visitString(ExprParser::StringContext *ctx) override;

private:
	  void enter(const char *fmt, ...);

	  void leave(const char *fmt, ...);

	  void error(const char *fmt, ...);

	  void todo(const char *fmt, ...);


private:
	  IItemFactory			*m_factory;
	  IBaseItem				*m_scope;

};
