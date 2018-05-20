/*
 * Z3ModelConstraintBuilder.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "PSIVisitor.h"
#include "Z3ExprTerm.h"

class Z3ModelBuilder;


class Z3ModelConstraintBuilder : public PSIVisitor {
public:
	Z3ModelConstraintBuilder(Z3ModelBuilder *builder);

	virtual ~Z3ModelConstraintBuilder();

	Z3ExprTerm build(IConstraint *c);

	virtual void visit_constraint_expr_stmt(IConstraintExpr *c) override;

	virtual void visit_constraint_if_stmt(IConstraintIf *c) override;


private:
	Z3ModelBuilder				*m_builder;
	Z3ExprTerm					m_expr;
	std::vector<Z3_ast>			m_if_else_conds;
};

