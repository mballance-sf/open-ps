/*
 * Z3ModelProcessor.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#include "Z3ModelProcessor.h"
#include "ExprEvaluator.h"
#include <limits.h>


Z3ModelProcessor::Z3ModelProcessor() {
	m_expr_depth = 0;
	m_exec_listener = 0;
	m_model = 0;
	m_root_comp = 0;
	m_root_action = 0;
}

Z3ModelProcessor::~Z3ModelProcessor() {
	// TODO Auto-generated destructor stub
}

bool Z3ModelProcessor::build(IComponent *comp, IAction *action) {
	m_root_comp   = comp;
	m_root_action = action;

//	m_strtab.build(comp);
//	m_strtab.build(action);
//
//	// TODO: this is the builder
//	push_prefix(action->getName());
//	visit_action(action);
//	pop_prefix();

	return true;
}

bool Z3ModelProcessor::run() {
	if (!m_model->check()) {
		fprintf(stdout, "Error: system doesn't solve\n");
		return false;
	}

	exec_action(
			m_root_action->getName(),
			m_root_action);

	return true;
}

void Z3ModelProcessor::set_exec_listener(IExecListener *l) {
	m_exec_listener = l;
}





void Z3ModelProcessor::collect_variables(
		std::vector<Z3ModelVar *>		&vars,
		const std::string				&context,
		IAction							*action) {
//	for (uint32_t i=0; i<action->getItems().size(); i++) {
//		IBaseItem *item = action->getItems().at(i);
//
//		if (dynamic_cast<IField *>(item)) {
//			IField *field = dynamic_cast<IField *>(item);
//			fprintf(stdout, "collect_variables: field=%s\n", field->getName().c_str());
//
//			if (dynamic_cast<IScalarType *>(field->getDataType())) {
//				// scalar field
//				std::string varname = context;
//				varname.append(".");
//				varname.append(field->getName());
//				if (m_variables.find(varname) != m_variables.end()) {
//					vars.push_back(m_variables.find(varname)->second);
//				} else {
//					fprintf(stdout, "Error: failed to find variable %s\n", varname.c_str());
//				}
//			} else {
//				// composite or user-defined field
//
//			}
//		}
//	}
}

// In this case, we only collect variables that will
// be randomized at the beginning of this action
void Z3ModelProcessor::collect_rand_variables(
		std::vector<Z3ModelVar *>		&vars,
		const std::string				&context,
		IAction							*action) {
//	for (uint32_t i=0; i<action->getItems().size(); i++) {
//		IBaseItem *item = action->getItems().at(i);
//
//		if (dynamic_cast<IField *>(item)) {
//			IField *field = dynamic_cast<IField *>(item);
//
//			if (dynamic_cast<IScalarType *>(field->getDataType())) {
//				// scalar field
//				std::string varname = context;
//				varname.append(".");
//				varname.append(field->getName());
//				std::map<std::string,Z3ModelVar *>::iterator it =
//						m_variables.find(varname);
//
//				if (it != m_variables.end()) {
//					vars.push_back(it->second);
//				}
//			} else {
//				// composite or user-defined field
//
//			}
//		}
//	}
}

void Z3ModelProcessor::exec_activity_stmt(
		const std::string			&context,
		IActivityStmt				*stmt) {
	switch (stmt->getStmtType()) {
	case IActivityStmt::ActivityStmt_Block: {
		IActivityBlockStmt *block = dynamic_cast<IActivityBlockStmt *>(stmt);
		fprintf(stdout, "block=%p\n", block);
		fflush(stdout);
		for (std::vector<IActivityStmt *>::const_iterator it=block->getStmts().begin();
				it!=block->getStmts().end(); it++) {
			exec_activity_stmt(context, *it);
		}
	} break;
	case IActivityStmt::ActivityStmt_Traverse: {
		exec_activity_traverse_stmt(context,
				dynamic_cast<IActivityTraverseStmt *>(stmt));
	} break;

	case IActivityStmt::ActivityStmt_Repeat: {
		exec_activity_repeat_stmt(context,
				dynamic_cast<IActivityRepeatStmt *>(stmt));
	} break;

	default: {
		fprintf(stdout, "TODO: unhandled activity-stmt %d\n",
				stmt->getStmtType());
	}
	}
}

void Z3ModelProcessor::exec_activity_traverse_stmt(
			const std::string				&context,
			IActivityTraverseStmt			*stmt) {
	IVariableRef *ref = stmt->getAction();
	fprintf(stdout, "ref=%p target=%p\n", ref, ref->getTarget());
	fflush(stdout);

	IField *action_f = dynamic_cast<IField *>(ref->getTarget());
	fprintf(stdout, "action_f=%p\n", action_f);
	fflush(stdout);
	IRefType *type_r = dynamic_cast<IRefType *>(action_f->getDataType());
	IAction *action = dynamic_cast<IAction *>(type_r->getTargetType());

	fprintf(stdout, "action_f.type=%d\n", action_f->getDataType()->getType());
	fflush(stdout);

	std::string this_ctxt = context + "." + ref->getId();

	exec_action(this_ctxt, action);
}

void Z3ModelProcessor::exec_activity_repeat_stmt(
		const std::string						&context,
		IActivityRepeatStmt						*repeat) {
//	ExprEvaluator eval(this);
//
//
//	switch (repeat->getRepeatType()) {
//	case IActivityRepeatStmt::RepeatType_Count: {
//		VarVal cond = eval.eval(context, repeat->getCond());
//		fprintf(stdout, "Repeat condition: %lld\n", cond.ui);
//
//		for (uint32_t i=0; i<cond.ui; i++) {
//			exec_activity_stmt(context, repeat->getBody());
//		}
//	} break;
//	default: {
//		fprintf(stdout, "Error: unsupported repeat type %d\n",
//				repeat->getRepeatType());
//	} break;
//	}


}

