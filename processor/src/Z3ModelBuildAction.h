/*
 * Z3ModelBuildAction.h
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#pragma once
#include "ModelVisitor.h"
#include "Z3ExprTerm.h"
#include "z3.h"

class Z3ModelBuilder;
class Z3ModelBuildAction : public virtual ModelVisitor {
public:

	Z3ModelBuildAction(Z3ModelBuilder *builder);

	virtual ~Z3ModelBuildAction();

	void build(IAction *action);

	virtual void visit_constraint(IConstraintBlock *c) override ;

	virtual void visit_field(IField *f) override;

	virtual void visit_activity(IActivityStmt *activity) override;

	virtual void visit_activity_select_stmt(IActivitySelectStmt *s) override;

	virtual void visit_activity_select_branch_stmt(IActivitySelectBranchStmt *s) override;


private:
	uint32_t				m_branch_idx;
	Z3ExprTerm				m_expr;
	Z3ModelBuilder			*m_builder;
};

