/*
 * Z3ModelBuilder.h
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
#pragma once
#include "Z3Model.h"
#include "IModel.h"
#include "ModelVisitor.h"
#include "IStringTable.h"
#include "StringTable.h"
#include "Z3ModelBuildAction.h"
#include "Z3ModelBuildExpr.h"
#include "Z3ModelBuildField.h"
#include "Z3ModelBuildConstraint.h"
#include "ModelNameProvider.h"
#include "z3.h"
#include <memory>


class Z3ModelBuilder : public virtual ModelVisitor {
public:
	Z3ModelBuilder();

	virtual ~Z3ModelBuilder();


	Z3ModelH build(
			IModel		*model,
			IComponent	*root_component,
			IAction		*root_action);

	Z3ModelBuildAction &action_builder() { return m_action_builder; }

	Z3ModelBuildExpr &expr_builder() { return m_expr_builder; }

	Z3ModelBuildField &field_builder() { return m_field_builder; }

	Z3ModelBuildConstraint &constraint_builder() {
		return m_constraint_builder;
	}

	ModelNameProvider &name_provider() {
		return m_name_provider;
	}

	virtual void visit_constraint(IConstraintBlock *c) override;

	virtual void visit_field(IField *f) override;

	Z3_context ctxt() const;

	Z3_solver solver() const;

	IStringTable &strtab() { return *m_strtab; } // TODO:

	void add_variable(Z3ModelVar *var);

	Z3ModelVar *get_variable(const std::string &name);

	void add_assert(const Z3ExprTerm &t);

	void inc_expr_depth();

	void dec_expr_depth();

	uint32_t expr_depth() const;

private:
	Z3ModelBuildAction				m_action_builder;
	Z3ModelBuildExpr				m_expr_builder;
	Z3ModelBuildField				m_field_builder;
	Z3ModelBuildConstraint			m_constraint_builder;
	IStringTableH					m_strtab;
	ModelNameProvider				m_name_provider;
	Z3Model							*m_z3_model;
	std::vector<std::string>		m_prefix_v;
	std::string						m_prefix;
	bool							m_prefix_valid;


};

