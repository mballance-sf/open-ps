/*
 * Expr2PSIVisitor.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 *
 *  Created on: Apr 20, 2018
 *      Author: ballance
 */
#pragma once
#include "ExprBaseVisitor.h"
#include "IItemFactory.h"



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
