/*
 * Z3ModelEvalAction.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include "IAction.h"
#include "PSIVisitor.h"
#include <string>

class Z3ModelEvaluator;
class Z3ModelVar;
class Z3ModelEvalAction : public virtual PSIVisitor {
public:
	Z3ModelEvalAction(Z3ModelEvaluator *evaluator);

	virtual ~Z3ModelEvalAction();

	void eval_action(IAction 				*action);

protected:

	virtual void visit_activity_if_else_stmt(IActivityIfElseStmt *stmt);

	virtual void visit_activity_parallel_block_stmt(IActivityBlockStmt *block);

	virtual void visit_activity_repeat_stmt(IActivityRepeatStmt *repeat);

	virtual void visit_activity_schedule_block_stmt(IActivityBlockStmt *s);

	virtual void visit_activity_select_stmt(IActivityBlockStmt *s);

	virtual void visit_activity_traverse_stmt(IActivityTraverseStmt *t);

	virtual void visit_activity_do_action_stmt(IActivityDoActionStmt *stmt);


private:

	void init_variables(
			IAction							*action);

	void collect_rand_variables(
			std::vector<Z3ModelVar *>		&vars,
			IAction							*action);


private:
	Z3ModelEvaluator			*m_evaluator;


};

