/*
 * Z3ModelBuilder.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelBuilder.h"
#include "StringTableBuilder.h"

Z3ModelBuilder::Z3ModelBuilder() : m_prefix_valid(false),
	m_expr_builder(this),
	m_field_builder(this),
	m_constraint_builder(this),
	m_strtab(0),
	m_z3_model(0) {


}

Z3ModelBuilder::~Z3ModelBuilder() {
}

Z3ModelH Z3ModelBuilder::build(
		IModel			*model,
		IComponent		*root_component,
		IAction			*root_action
		) {

	StringTableBuilder strtab_builder;
	m_strtab = strtab_builder.build(root_component, root_action);

	m_z3_model = new Z3Model(m_strtab, root_action);

	m_name_provider.enter(root_action->getName());
	visit_action(root_action);
	m_name_provider.leave(root_action->getName());


	fprintf(stdout, "Model:\n%s\n", m_z3_model->toString().c_str());


	return Z3ModelH(m_z3_model);
}

void Z3ModelBuilder::visit_constraint(IConstraintBlock *c) {
	Z3ExprTerm term = m_constraint_builder.build(c);

	Z3_solver_assert(
			m_z3_model->ctxt(),
			m_z3_model->solver(),
			term.expr());

//	fprintf(stdout, "--> Z3_solver_get_model\n");
//	fflush(stdout);
//	Z3_model m = Z3_solver_get_model(
//			m_z3_model->ctxt(),
//			m_z3_model->solver());
//	fprintf(stdout, "<-- Z3_solver_get_model\n");
//	fflush(stdout);
//	Z3_model_inc_ref(m_z3_model->ctxt(), m);
//	std::string m_s = Z3_model_to_string(
//			m_z3_model->ctxt(), m);
//	Z3_model_dec_ref(m_z3_model->ctxt(), m);
//
//	fprintf(stdout, "Model:\n%s\n", m_s.c_str());
}

void Z3ModelBuilder::visit_field(IField *f) {
	m_field_builder.build(f);
}

Z3_context Z3ModelBuilder::ctxt() const {
	return m_z3_model->ctxt();

}

Z3_solver Z3ModelBuilder::solver() const {
	return m_z3_model->solver();
}

void Z3ModelBuilder::add_variable(Z3ModelVar *var) {
	m_z3_model->add_variable(var);
}

Z3ModelVar *Z3ModelBuilder::get_variable(const std::string &name) {
	return m_z3_model->get_variable(name);
}



