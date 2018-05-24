/*
 * Z3ModelBuildConstraint.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "PSIVisitor.h"
#include "Z3ExprTerm.h"

class Z3ModelBuilder;


class Z3ModelBuildConstraint : public PSIVisitor {
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

