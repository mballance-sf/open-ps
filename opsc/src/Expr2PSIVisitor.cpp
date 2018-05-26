/*
 * Expr2PSIVisitor.cpp
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

#include "Expr2PSIVisitor.h"
#include <stdarg.h>

Expr2PSIVisitor::Expr2PSIVisitor(
		IItemFactory 	*factory,
		IBaseItem		*scope) : m_factory(factory), m_scope(scope) {
	// TODO Auto-generated constructor stub

}

Expr2PSIVisitor::~Expr2PSIVisitor() {
	// TODO Auto-generated destructor stub
}

antlrcpp::Any Expr2PSIVisitor::visitEntry(ExprParser::EntryContext *ctx) {
	IExpr *ret = 0;

	enter("visitEntry");
	ret = ctx->expression()->accept(this);
	leave("visitEntry");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitOpen_range_list(ExprParser::Open_range_listContext *ctx) {
	IOpenRangeList *ret = 0;
	std::vector<IOpenRangeValue *> ranges;

	enter("visitOpen_range_list");

	for (uint32_t i=0; i<ctx->open_range_value().size(); i++) {
		ranges.push_back(ctx->open_range_value(i)->accept(this));
	}
	ret = m_factory->mkOpenRangeList(ranges);

	leave("visitOpen_range_list");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitOpen_range_value(ExprParser::Open_range_valueContext *ctx) {
	IOpenRangeValue *ret = 0;
	IExpr *lhs=0, *rhs=0;

	enter("visitOpen_range_value");

	lhs = ctx->lhs->accept(this);
	if (ctx->rhs) {
		rhs = ctx->rhs->accept(this);
	}

	ret = m_factory->mkOpenRangeValue(lhs, rhs);

	leave("visitOpen_range_value");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitExpression(ExprParser::ExpressionContext *ctx) {
	bool is_bin_op;
	IExpr *ret = 0;

	enter("visitExpression");

	if (ctx->unary_op()) {
		todo("unary op");
		ret = ctx->lhs->accept(this);
	} else if (ctx->exp_op()) {
		// TODO:
		todo("exp op");
	} else if (ctx->mul_div_mod_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->mul_div_mod_op()->getText() == "*") {
				op = IBinaryExpr::BinOp_Multiply;
			} else if (ctx->mul_div_mod_op()->getText() == "/") {
				op = IBinaryExpr::BinOp_Divide;
			} else if (ctx->mul_div_mod_op()->getText() == "%") {
				op = IBinaryExpr::BinOp_Mod;
			}
		IExpr *lhs = ctx->lhs->accept(this);
		IExpr *rhs = ctx->rhs->accept(this);

		ret = m_factory->mkBinExpr(lhs, op, rhs);
	} else if (ctx->add_sub_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->add_sub_op()->getText() == "+") {
				op = IBinaryExpr::BinOp_Plus;
			} else if (ctx->add_sub_op()->getText() == "-") {
				op = IBinaryExpr::BinOp_Minus;
			}

		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->shift_op()) {
		todo("shift_op");
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
			if (ctx->shift_op()->getText() == "<<") {
//				op = IBinaryExpr::BinOp_ TODO
			} else if (ctx->shift_op()->getText() == ">>") {
//				op = IBinaryExpr::BinOp_ TODO
			}
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->eq_neq_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
		if (ctx->eq_neq_op()->getText() == "==") {
			op = IBinaryExpr::BinOp_EqEq;
		} else if (ctx->eq_neq_op()->getText() == "!=") {
			op = IBinaryExpr::BinOp_NotEq;
		}
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->logical_inequality_op()) {
		IBinaryExpr::BinOpType op = IBinaryExpr::BinOp_Eq;
		if (ctx->logical_inequality_op()->getText() == "<") {
			op = IBinaryExpr::BinOp_LT;
		} else if (ctx->logical_inequality_op()->getText() == "<=") {
			op = IBinaryExpr::BinOp_LE;
		} else if (ctx->logical_inequality_op()->getText() == ">") {
			op = IBinaryExpr::BinOp_GT;
		} else if (ctx->logical_inequality_op()->getText() == ">=") {
			op = IBinaryExpr::BinOp_GE;
		}
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), op,
				ctx->rhs->accept(this));
	} else if (ctx->binary_and_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_And,
				ctx->rhs->accept(this));
	} else if (ctx->binary_xor_op()) {
		todo("binary_xor_op");
		// TODO:
//		ret = m_factory->mkBinExpr(
//				ctx->lhs->accept(this), IBinaryExpr::BinOp_Xor,
//				ctx->rhs->accept(this));
	} else if (ctx->binary_or_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_Or,
				ctx->rhs->accept(this));
	} else if (ctx->logical_and_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_AndAnd,
				ctx->rhs->accept(this));
	} else if (ctx->logical_or_op()) {
		ret = m_factory->mkBinExpr(
				ctx->lhs->accept(this), IBinaryExpr::BinOp_OrOr,
				ctx->rhs->accept(this));
	} else if (ctx->primary()) {
		ret = ctx->primary()->accept(this);
	} else if (ctx->inside_expr_term()) {
		IExpr *lhs = ctx->lhs->accept(this);
		IOpenRangeList *rhs = ctx->inside_expr_term()->open_range_list()->accept(this);
		ret = m_factory->mkInExpr(lhs, rhs);
	} else {
		todo("unknown expression %s\n", ctx->getText().c_str());
		enter("unknown %s\n", ctx->getText().c_str());
		/* ret = */ visitChildren(ctx);
		leave("unknown");
	}

	leave("visitExpression");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitConditional_expr(ExprParser::Conditional_exprContext *ctx) {
	return (IExpr *)0;
}

antlrcpp::Any Expr2PSIVisitor::visitStatic_ref_path(ExprParser::Static_ref_pathContext *ctx) {
	IExpr *ret = 0;
	enter("visitStatic_ref_path");
	todo("visitStatic_ref_path");
	leave("visitStatic_ref_path");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitVariable_ref_path(ExprParser::Variable_ref_pathContext *ctx) {
	IExpr *ret = 0;

	enter("visitVariable_ref_path");
	IVariableRef *vref = 0;
	IVariableRef *prev = 0;
	IBaseItem *s = dynamic_cast<IBaseItem *>(m_scope);

	for (uint32_t i=0; i<ctx->variable_ref().size(); i++) {
		ExprParser::Variable_refContext *ref_ctx = ctx->variable_ref(i);
		const std::string &id = ref_ctx->identifier()->getText();
		IExpr *index_lhs = 0, *index_rhs = 0;
		if (ref_ctx->expression(0)) {
			index_lhs = ref_ctx->expression(0)->accept(this);
		}
		if (ref_ctx->expression(1)) {
			index_rhs = ref_ctx->expression(1)->accept(this);
		}

		IVariableRef *ref = m_factory->mkVariableRef(s, id, index_lhs, index_rhs);

		if (i == 0) {
			vref = ref;
		}
		if (prev) {
			prev->setNext(ref);
		}
		prev = ref;
	}

	ret = vref;
	leave("visitVariable_ref_path");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitBool_literal(ExprParser::Bool_literalContext *ctx) {
	IExpr *ret = 0;

	enter("visitBool_literal");
	ret = m_factory->mkBoolLiteral(ctx->getText() == "true");
	leave("visitBool_literal");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitNumber(ExprParser::NumberContext *ctx) {
	IExpr *ret = 0;

	enter("visitNumber");

	uint64_t v = 0;
	if (ctx->dec_number()) {
		v = strtoul(ctx->getText().c_str(), 0, 0);
	} else if (ctx->hex_number()) {
		const char *t = ctx->getText().c_str();
		v = strtoul(&t[2], 0, 16);
	} else if (ctx->oct_number()) {
		v = strtoul(ctx->getText().c_str(), 0, 8);
	} else {
		error("unsupported number format");
	}
	ret = m_factory->mkBitLiteral(v); // TODO:

	leave("visitNumber");

	return ret;
}

antlrcpp::Any Expr2PSIVisitor::visitString(ExprParser::StringContext *ctx) {
	IExpr *ret = 0;

	enter("visitString");
	const std::string &s = ctx->getText();
	ret = m_factory->mkStringLiteral(s.substr(1, s.size()-2));
	leave("visitString");

	return ret;
}

void Expr2PSIVisitor::enter(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "--> ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void Expr2PSIVisitor::leave(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "<-- ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void Expr2PSIVisitor::error(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

void Expr2PSIVisitor::todo(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "TODO: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}
