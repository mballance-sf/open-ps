/*
 * Z3ModelBuildConstraint.cpp
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

void Z3ModelBuildConstraint::visit_constraint_implies_stmt(IConstraintImplies *c) {
	Z3ExprTerm cond = m_builder->expr_builder().build(c->getCond());
	Z3ExprTerm rhs = build(c->getImp());

	m_expr = Z3ExprTerm(Z3_mk_implies(m_builder->ctxt(),
			cond.expr(),
			rhs.expr()),
			1, // TODO:
			true);
}

void Z3ModelBuildConstraint::visit_constraint_foreach_stmt(IConstraintForeach *c) {
	fprintf(stdout, "TODO: visit_constraint_foreach_stmt\n");
}

void Z3ModelBuildConstraint::visit_constraint_unique_stmt(IConstraintUnique *c) {
	std::vector<Z3_ast> u_l;

	for (uint32_t i=0; i<c->getTarget()->ranges().size(); i++) {
		Z3ExprTerm e = m_builder->expr_builder().build(
				c->getTarget()->ranges().at(i)->getLHS());
		u_l.push_back(e.expr());
	}

	m_expr = Z3ExprTerm(
			Z3_mk_distinct(m_builder->ctxt(), u_l.size(), u_l.data()),
			1,
			false);
}

void Z3ModelBuildConstraint::visit_constraint_block(IConstraintBlock *block) {
	std::vector<Z3_ast> c_l;

	for (uint32_t i=0; i<block->getConstraints().size(); i++) {
		visit_constraint_stmt(block->getConstraints().at(i));
		Z3_ast e = m_expr.expr();
		Z3_sort s = Z3_get_sort(m_builder->ctxt(), e);
		Z3_sort_kind sk = Z3_get_sort_kind(m_builder->ctxt(), s);

		fprintf(stdout, "Sort Kind: %d\n", sk);

		// TODO: construct a bool if needed
		c_l.push_back(e);
	}

	m_expr = Z3ExprTerm(Z3_mk_and(m_builder->ctxt(),
					c_l.size(),
					c_l.data()),
			1, // TODO:
			false);
}



