/*
 * Z3ModelBuilder.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */
#pragma once
#include "Z3Model.h"
#include "IModel.h"
#include "PSIVisitor.h"
#include "IStringTable.h"
#include "StringTable.h"
#include "Z3ModelBuildExpr.h"
#include "Z3ModelBuildField.h"
#include "Z3ModelBuildConstraint.h"
#include "z3.h"
#include <memory>


class Z3ModelBuilder : public virtual PSIVisitor {
public:
	Z3ModelBuilder();

	virtual ~Z3ModelBuilder();


	Z3ModelH build(
			IModel		*model,
			IComponent	*root_component,
			IAction		*root_action);

	Z3ModelBuildExpr &expr_builder() { return m_expr_builder; }

	Z3ModelBuildField &field_builder() { return m_field_builder; }

	Z3ModelBuildConstraint &constraint_builder() {
		return m_constraint_builder;
	}

	Z3_context ctxt() const { return 0; } // TODO:

	Z3_solver solver() const { return 0; } // TODO:

	IStringTable &strtab() { return *m_strtab; } // TODO:

	void add_variable(Z3ModelVar *var);

	Z3ModelVar *get_variable(const std::string &name);

	void inc_expr_depth();

	void dec_expr_depth();

	uint32_t expr_depth() const;

	void push_prefix(const std::string &pref);

	void pop_prefix();

	const std::string &prefix();

private:
	Z3ModelBuildExpr				m_expr_builder;
	Z3ModelBuildField				m_field_builder;
	Z3ModelBuildConstraint			m_constraint_builder;
	IStringTableH					m_strtab;
	Z3Model							*m_z3_model;
	std::vector<std::string>		m_prefix_v;
	std::string						m_prefix;
	bool							m_prefix_valid;


};

