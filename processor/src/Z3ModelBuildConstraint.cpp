/*
 * Z3ModelBuildConstraint.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelBuildConstraint.h"
#include "Z3ModelBuilder.h"

Z3ModelBuildConstraint::Z3ModelBuildConstraint(Z3ModelBuilder *builder) : m_builder(builder) {
	// TODO Auto-generated constructor stub

}

Z3ModelBuildConstraint::~Z3ModelBuildConstraint() {
	// TODO Auto-generated destructor stub
}

Z3ExprTerm Z3ModelBuildConstraint::build(IConstraint *c) {
	visit_constraint_stmt(c);
	return m_expr;
}


void Z3ModelBuildConstraint::visit_constraint_expr_stmt(IConstraintExpr *c) {
	m_expr = m_builder->expr_builder().build(c->getExpr());

//	if (m_builder->expr_depth() == 0) {
//		if (m_expr.expr()) {
//			//		fprintf(stdout, "AST:\n%s\n",
//			//				Z3_ast_to_string(m_ctxt, m_expr.expr()));
//			Z3_solver_assert(m_builder->ctxt(), m_builder->solver(),
//					m_expr.expr());
//		} else {
//			fprintf(stdout, "Error: expr resulted in null term\n");
//		}
//	}

//	fprintf(stdout, "constraint statement: %p\n", m_expr);
}

void Z3ModelBuildConstraint::visit_constraint_if_stmt(IConstraintIf *c) {
//	m_builder->inc_expr_depth();
	Z3ExprTerm iff = m_builder->expr_builder().build(c->getCond());
	visit_constraint_stmt(c->getTrue());
	Z3ExprTerm case_true = m_expr;

	Z3_ast iff_ast = iff.expr();

	if (c->getFalse()) {
		m_if_else_conds.push_back(iff_ast);

		// Create a NOT of
		visit_constraint_stmt(c->getFalse());
		Z3ExprTerm case_false = m_expr;

		m_expr = Z3ExprTerm(
				Z3_mk_ite(m_builder->ctxt(),
						iff.expr(),
						case_true.expr(),
						case_false.expr()),
						case_true.size(),
						case_true.is_signed()
				);
		m_if_else_conds.pop_back();
	} else {
		m_expr = Z3ExprTerm(
				Z3_mk_implies(m_builder->ctxt(),
						iff.expr(),
						case_true.expr()),
						case_true.size(),
						case_true.is_signed());
//		Z3_solver_assert(
//				m_builder->ctxt(),
//				m_builder->solver(), ast);
	}

//	m_builder->dec_expr_depth();
}


