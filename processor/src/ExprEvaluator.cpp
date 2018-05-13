/*
 * ExprEvaluator.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: ballance
 */

#include "ExprEvaluator.h"

ExprEvaluator::ExprEvaluator(IVarValueProvider *p) : m_var_provider(p) {

}

ExprEvaluator::~ExprEvaluator() {
	// TODO Auto-generated destructor stub
}

const VarVal &ExprEvaluator::eval(const std::string &context, IExpr *expr) {
	fprintf(stdout, "eval: %p\n", expr);
	fflush(stdout);
	m_context.clear();
	if (context != "") {
		m_context.push_back(context);
	}

	visit_expr(expr);

	return m_val;
}

void ExprEvaluator::visit_binary_expr(IBinaryExpr *be) {
	fprintf(stdout, "visit_binary_expr: %p\n", be);
	fflush(stdout);
	visit_expr(be->getLHS());
	VarVal lhs = m_val;
	visit_expr(be->getRHS());
	VarVal rhs = m_val;

	switch (be->getBinOpType()) {
	case IBinaryExpr::BinOp_Plus: {
		m_val.ui = (lhs.ui + rhs.ui);
		m_val.type = lhs.type;
		m_val.w = (lhs.w>rhs.w)?lhs.w:rhs.w;
	} break;

	default:
		fprintf(stdout, "Error: unsupported expression-eval op %d\n",
				be->getBinOpType());
		break;

	}
}

void ExprEvaluator::visit_variable_ref(IVariableRef *ref) {
	fprintf(stdout, "visit_variable_ref: %p\n", ref);
	fflush(stdout);
	std::string ctxt = context();

	IVariableRef *r = ref;
	while (r) {
		if (ctxt != "") {
			ctxt.append(".");
		}
		ctxt.append(r->getId());

		r = r->getNext();
	}

	m_val = m_var_provider->get_value(ctxt);
}

void ExprEvaluator::visit_literal_expr(ILiteral *l) {
	fprintf(stdout, "visit_literal_expr: %p\n", l);
	fflush(stdout);
	switch (l->getLiteralType()) {
	case ILiteral::LiteralBit:
		m_val.type = VarVal_Uint;
		m_val.w = 64;
		m_val.ui = l->getBit();
		break;
	case ILiteral::LiteralInt:
		m_val.type = VarVal_Int;
		m_val.w = 64;
		m_val.si = l->getInt();
		break;
	case ILiteral::LiteralBool:
		m_val.type = VarVal_Bool;
		m_val.w = 1;
		m_val.b = l->getBool();
		break;

	case ILiteral::LiteralString:
		m_val.type = VarVal_String;
		m_val.w = 0;
		m_val.s = l->getString();
		break;
	}
}

std::string ExprEvaluator::context() {
	std::string ret;

	for (uint32_t i=0; i<m_context.size(); i++) {
		ret.append(m_context.at(i));

		if (i+1 < m_context.size()) {
			ret.append(".");
		}
	}

	return ret;
}

