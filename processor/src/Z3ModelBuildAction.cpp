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
	Z3ModelVar *select_var = m_builder->field_builder().mkIntVar(
			m_builder->name_provider().name() + ".__select_idx");
	std::vector<Z3_ast> branch_ast;

	// Limit index to 0..size-1 || -1
	Z3ModelBuildExpr &eb = m_builder->expr_builder();
	Z3ExprTerm sv_e = eb.mk_var(select_var);
	eb.mk_or(
			eb.mk_and(
					eb.mk_ge(
							sv_e,
							expr_builder.mk_int(0)),
					eb.mk_lt(
							sv_e,
							eb.mk_int(s->getBranches().size()))
			),
			eb.mk_int(-1)
		);


	fprintf(stdout, "visit_activity_select_stmt: name=%s\n",
			m_builder->name_provider().name().c_str());

	m_builder->name_provider().leave(s);
}

