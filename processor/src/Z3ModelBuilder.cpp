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

	return Z3ModelH(m_z3_model);
}

void Z3ModelBuilder::add_variable(Z3ModelVar *var) {
	m_z3_model->add_variable(var);
}

Z3ModelVar *Z3ModelBuilder::get_variable(const std::string &name) {
	return m_z3_model->get_variable(name);
}

void Z3ModelBuilder::push_prefix(const std::string &pref) {
	m_prefix_v.push_back(pref);
	m_prefix_valid = false;
}

void Z3ModelBuilder::pop_prefix() {
	m_prefix_v.pop_back();
	m_prefix_valid = false;
}

const std::string &Z3ModelBuilder::prefix() {

	if (!m_prefix_valid) {
		m_prefix.clear();
		for (uint32_t i=0; i<m_prefix_v.size(); i++) {
			m_prefix.append(m_prefix_v.at(i));
			if (i+1 < m_prefix_v.size()) {
				m_prefix.append(".");
			}
		}
		m_prefix_valid = true;
	}

	return m_prefix;
}


