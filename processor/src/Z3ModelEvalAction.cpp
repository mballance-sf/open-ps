/*
 * Z3ModelEvalAction.cpp
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#include <vector>
#include "Z3ModelVar.h"
#include "IExec.h"
#include "Z3ModelEvalAction.h"

Z3ModelEvalAction::Z3ModelEvalAction(Z3ModelEvaluator *evaluator) {
	m_evaluator = evaluator;
}

Z3ModelEvalAction::~Z3ModelEvalAction() {
	// TODO Auto-generated destructor stub
}

void Z3ModelEvalAction::eval_action(
		const std::string		&context,
		IAction 				*action) {
	std::vector<Z3ModelVar *> reset_vars;
	std::vector<Z3ModelVar *> rand_vars;
	IExec *pre_solve=0;
	IExec *post_solve=0;
	IExec *body=0;

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
//	// Solve for the random variables
//	if (rand_vars.size() > 0) {
//		if (!solve(rand_vars)) {
//			fprintf(stdout, "Error: solve failed\n");
//		}
//		// Now, lock down the values of the random variables
//		for (std::vector<Z3ModelVar *>::const_iterator it=rand_vars.begin();
//				it!=rand_vars.end(); it++) {
//			const VarVal &val = (*it)->get_val(m_ctxt, m_model);
//			fprintf(stdout, "%s = %lld\n",
//					(*it)->name().c_str(),
//					val.ui);
//			(*it)->set_val(val.ui);
//		}
//	}
//
//	if (post_solve) {
//		fprintf(stdout, "TODO: support post-solve exec\n");
//	}
//
//	if (body) {
//		if (m_exec_listener) {
//			m_exec_listener->exec(context, body);
//		} else {
//			fprintf(stdout, "Warning: no exec listener\n");
//		}
//	} else if (action->getActivity()) {
//		exec_activity_stmt(context, action->getActivity());
//	} else {
//		fprintf(stdout, "Note: this is a boring action with no exec and no activity\n");
//	}

	fprintf(stdout, "<-- exec_action\n");
}

