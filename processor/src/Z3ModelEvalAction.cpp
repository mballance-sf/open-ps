/*
 * Z3ModelEvalAction.cpp
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
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#include "Z3ModelEvalAction.h"
#include "Z3ModelEvaluator.h"
#include "ExprEvaluator.h"

#include <cstdint>
#include <cstdio>
#include <iterator>
#include <vector>

#include "IBaseItem.h"
#include "IExec.h"
#include "IField.h"
#include "IScalarType.h"
#include "Z3ModelVar.h"

Z3ModelEvalAction::Z3ModelEvalAction(Z3ModelEvaluator *evaluator) {
	m_evaluator = evaluator;
}

Z3ModelEvalAction::~Z3ModelEvalAction() {
	// TODO Auto-generated destructor stub
}

void Z3ModelEvalAction::eval_action(IAction	*action) {
	std::vector<Z3ModelVar *> reset_vars;
	std::vector<Z3ModelVar *> rand_vars;
	IExec *pre_solve=0;
	IExec *post_solve=0;
	IExec *body=0;
	std::string context = m_evaluator->name_provider().name();

	fprintf(stdout, "--> exec_action %s\n", context.c_str());

	for (std::vector<IBaseItem *>::const_iterator it=action->getItems().begin();
			it!=action->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeExec) {
			IExec *exec = dynamic_cast<IExec *>(*it);
			switch (exec->getExecKind()) {
			case IExec::Body: body = exec; break;
			case IExec::PreSolve: pre_solve = exec; break;
			case IExec::PostSolve: post_solve = exec; break;
			default:
				fprintf(stdout, "TODO: unsupported exec kind %d\n", exec->getExecKind());
				break;
			}
		}
	}

	if (pre_solve) {
		fprintf(stdout, "TODO: support pre-solve exec\n");
	}

	// Initialize variables in this scope and sub-scopes
	m_evaluator->var_initializer().init(action);

	// Collect variables to solve for
	rand_vars.clear();
	collect_rand_variables(rand_vars, action);

//	collect_variables(reset_vars, context, action);
//
//	// Collect variables that will be randomized in this action
//	collect_rand_variables(rand_vars, context, action);
//
//	// Reset all context actions
//	for (std::vector<Z3ModelVar *>::const_iterator it=reset_vars.begin();
//			it!=reset_vars.end(); it++) {
//		fprintf(stdout, "Reset: %s\n", (*it)->name().c_str());
//		(*it)->reset();
//	}
//
	// Solve for the random variables
	if (rand_vars.size() > 0) {
		if (!m_evaluator->solve(rand_vars)) {
			fprintf(stdout, "Error: solve failed\n");
		}

		for (std::vector<Z3ModelVar *>::const_iterator it=rand_vars.begin();
				it!=rand_vars.end(); it++) {
			const VarVal &val = m_evaluator->get_val(*it);
			fprintf(stdout, "%s = %lld\n",
					(*it)->name().c_str(),
					val.ui);
//			(*it)->set_val(val.ui);
		}
	}

	if (post_solve) {
		fprintf(stdout, "TODO: support post-solve exec\n");
	}

	if (body) {
		if (m_evaluator->exec_listener()) {
			m_evaluator->exec_listener()->exec(context, body);
		} else {
			fprintf(stdout, "Warning: no exec listener\n");
		}
	} else if (action->getActivity()) {
		fprintf(stdout, "--> visit_activity\n");
		visit_activity(action->getActivity());
		fprintf(stdout, "<-- visit_activity\n");
	} else {
		fprintf(stdout, "Note: this is a boring action with no exec and no activity\n");
	}

	fprintf(stdout, "<-- exec_action\n");
}

void Z3ModelEvalAction::collect_rand_variables(
		std::vector<Z3ModelVar *>		&vars,
		IAction							*action) {

	for (uint32_t i=0; i<action->getItems().size(); i++) {
		IBaseItem *item = action->getItems().at(i);

		if (dynamic_cast<IField *>(item)) {
			IField *field = dynamic_cast<IField *>(item);
			m_evaluator->name_provider().enter(field);
			fprintf(stdout, "collect_variables: field=%s\n", field->getName().c_str());

			if (dynamic_cast<IScalarType *>(field->getDataType())) {
				// scalar field
				std::string varname = m_evaluator->name_provider().name();
				Z3ModelVar *var = m_evaluator->get_variable(varname);

				if (var) {
					fprintf(stdout, "Add var %s\n", var->name().c_str());
					vars.push_back(var);
				} else {
					fprintf(stdout, "Error: failed to find variable %s\n", varname.c_str());
				}
			} else {
				// composite or user-defined field
				fprintf(stdout, "TODO: composite variable\n");
			}
			m_evaluator->name_provider().leave(field);
		}
	}
}

void Z3ModelEvalAction::visit_activity_if_else_stmt(IActivityIfElseStmt *stmt) {
	fprintf(stdout, "TODO: activity_if_else_stmt\n");
}

void Z3ModelEvalAction::visit_activity_parallel_block_stmt(IActivityBlockStmt *block) {
	fprintf(stdout, "TODO: activity_parallel_block_stmt\n");
}

void Z3ModelEvalAction::visit_activity_repeat_stmt(IActivityRepeatStmt *repeat) {
	fprintf(stdout, "--> visit_activity_repeat_stmt\n");

	switch (repeat->getRepeatType()) {
	case IActivityRepeatStmt::RepeatType_Count: {
		ExprEvaluator eval(m_evaluator->model());
		VarVal cond = eval.eval(
				m_evaluator->name_provider().name(),
				repeat->getCond());

		for (uint32_t i=0; i<cond.ui; i++) {
			visit_activity_stmt(repeat->getBody());
		}
	} break;
	default: {
		fprintf(stdout, "Error: unsupported repeat type %d\n",
				repeat->getRepeatType());
	}
	}

	fprintf(stdout, "<-- visit_activity_repeat_stmt\n");
}

void Z3ModelEvalAction::visit_activity_schedule_block_stmt(IActivityBlockStmt *s) {
	fprintf(stdout, "TODO: activity_schedule_block_stmt\n");
}

void Z3ModelEvalAction::visit_activity_select_stmt(IActivitySelectStmt *s) {
	m_evaluator->name_provider().enter(s);
	Z3ModelVar *select_var = m_evaluator->get_variable(
			m_evaluator->name_provider().name() + ".__select_idx");
	fprintf(stdout, "TODO: activity_select_stmt %p\n", select_var);
	m_evaluator->solve(select_var);
	fprintf(stdout, "  select: %lld\n", m_evaluator->get_val(select_var).si);

	m_evaluator->name_provider().leave(s);
}

void Z3ModelEvalAction::visit_activity_traverse_stmt(IActivityTraverseStmt *stmt) {
	fprintf(stdout, "--> visit_activity_traverse_stmt\n");

	IVariableRef *ref = stmt->getAction();
	IField *action_f = dynamic_cast<IField *>(ref->getTarget());
	IRefType *type_r = dynamic_cast<IRefType *>(action_f->getDataType());
	IAction *action = dynamic_cast<IAction *>(type_r->getTargetType());

	m_evaluator->name_provider().enter(action_f);
	eval_action(action);
	m_evaluator->name_provider().leave(action_f);

	fprintf(stdout, "<-- visit_activity_traverse_stmt\n");
}

void Z3ModelEvalAction::visit_activity_do_action_stmt(IActivityDoActionStmt *stmt) {
	fprintf(stdout, "TODO: activity_do_action_stmt\n");
}


void Z3ModelEvalAction::init_variables(IAction	*action) {
	fprintf(stdout, "TODO: initialize actions\n");
}

