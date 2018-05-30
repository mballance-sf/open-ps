/*
 * Z3ModelBuildAction.cpp
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#include "Z3ModelBuildAction.h"
#include "Z3ModelBuilder.h"

Z3ModelBuildAction::Z3ModelBuildAction(Z3ModelBuilder *builder) : m_builder(builder) {

}

Z3ModelBuildAction::~Z3ModelBuildAction() {
	// TODO Auto-generated destructor stub
}

void Z3ModelBuildAction::build(IAction *action) {
	visit_action(action);
}

void Z3ModelBuildAction::visit_constraint(IConstraintBlock *c) {
	m_builder->visit_constraint(c);
}

void Z3ModelBuildAction::visit_field(IField *f) {
	m_builder->visit_field(f);
}

void Z3ModelBuildAction::visit_activity(IActivityStmt *activity) {
	ModelVisitor::visit_activity(activity);
}

void Z3ModelBuildAction::visit_activity_select_stmt(IActivitySelectStmt *s) {
	m_builder->name_provider().enter(s);
	std::vector<Z3_ast> branch_ast;
	uint32_t sel_bits = 32;
	uint32_t sel_max = s->getBranches().size();

	for (sel_bits=0; sel_max>0; sel_bits++) {
		sel_max >>= 1;
	}
	sel_bits++; // Account for sign
	fprintf(stdout, "sel_max=%d sel_bits=%d\n", s->getBranches().size(), sel_bits);
	Z3ModelVar *select_var = m_builder->field_builder().mkIntVar(
			m_builder->name_provider().name() + ".__select_idx", sel_bits);

	// Limit index to 0..size-1 || -1
	Z3ModelBuildExpr &eb = m_builder->expr_builder();
	Z3ExprTerm sv_e = eb.mk_var(select_var);
//	Z3ExprTerm select_c = eb.mk_logical_or(
//			eb.mk_logical_and(
//					eb.mk_ge(
//							sv_e,
//							eb.mk_int(0)),
//						eb.mk_lt(
//								sv_e,
//								eb.mk_int(s->getBranches().size()))
//			),
//			eb.mk_eq(sv_e, eb.mk_int(-1))
//	);
	Z3ExprTerm select_c =
			eb.mk_logical_and(
					eb.mk_ge(
							sv_e,
							eb.mk_int(0)),
						eb.mk_lt(
								sv_e,
								eb.mk_int(s->getBranches().size()))
			);
	m_builder->add_assert(select_c);
//	Z3ExprTerm tmp = eb.mk_eq(sv_e, eb.mk_int(s->getBranches().size()-1));
//	m_builder->add_assert(tmp);

	// Build each branch, and the conditions under which
	// the branch may be executed
	std::vector<Z3_ast> pos_l;
	std::vector<Z3_ast> neg_l;
	for (uint32_t i=0; i<s->getBranches().size(); i++) {
		// select_var==i -> branch_constraint
		m_branch_idx = i;
		visit_activity_select_branch_stmt(s->getBranches().at(i));
	}

	fprintf(stdout, "visit_activity_select_stmt: name=%s\n",
			m_builder->name_provider().name().c_str());

	m_builder->name_provider().leave(s);
}

void Z3ModelBuildAction::visit_activity_select_branch_stmt(IActivitySelectBranchStmt *s) {

}

