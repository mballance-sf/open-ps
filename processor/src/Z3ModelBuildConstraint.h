/*
 * Z3ModelBuildConstraint.h
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
#include "ModelVisitor.h"
#include "Z3ExprTerm.h"

class Z3ModelBuilder;


class Z3ModelBuildConstraint : public ModelVisitor {
public:
	Z3ModelBuildConstraint(Z3ModelBuilder *builder);

	virtual ~Z3ModelBuildConstraint();

	Z3ExprTerm build(IConstraint *c);

	virtual void visit_constraint_expr_stmt(IConstraintExpr *c) override;

	virtual void visit_constraint_if_stmt(IConstraintIf *c) override;

	virtual void visit_constraint_implies_stmt(IConstraintImplies *c) override;

	virtual void visit_constraint_foreach_stmt(IConstraintForeach *c) override;

	virtual void visit_constraint_unique_stmt(IConstraintUnique *c) override;

	virtual void visit_constraint_block(IConstraintBlock *block) override;


private:
	Z3ModelBuilder				*m_builder;
	Z3ExprTerm					m_expr;
	std::vector<Z3_ast>			m_if_else_conds;
};

