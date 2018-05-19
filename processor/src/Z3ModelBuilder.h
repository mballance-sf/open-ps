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
#include "Z3ModelExprBuilder.h"
#include "IStringTable.h"
#include "z3.h"

using namespace psi_api;
using namespace psi::apps;

class Z3ModelBuilder : public virtual PSIVisitor {
public:
	Z3ModelBuilder();

	virtual ~Z3ModelBuilder();


	Z3Model *build(
			IModel		*model,
			IComponent	*root_component,
			IAction		*root_action);

	Z3ModelExprBuilder *expr_builder() const { return m_expr_builder; }

	Z3_context ctxt() const { return 0; } // TODO:

	Z3_solver solver() const { return 0; } // TODO:

	IStringTable *strtab() const { return 0; } // TODO:

	void add_variable(Z3ModelVar *var);

	void inc_expr_depth();

	void dec_expr_depth();

	uint32_t expr_depth() const;

	void push_prefix(const std::string &pref);

	void pop_prefix();

	const std::string &prefix();

private:
	Z3ModelExprBuilder				*m_expr_builder;
	std::vector<std::string>		m_prefix_v;
	std::string						m_prefix;
	bool							m_prefix_valid;


};

