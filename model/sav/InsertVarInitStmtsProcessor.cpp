/*
 * InsertVarInitStmtsProcessor.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "InsertVarInitStmtsProcessor.h"
#include "ExecImpl.h"
#include "ActionImpl.h"
#include "StructImpl.h"
#include "ExecExprStmtImpl.h"
#include "FieldRefImpl.h"
#include "RulesTempVar.h"
#include "LiteralImpl.h"
#include <stdio.h>

namespace qpssc {

InsertVarInitStmtsProcessor::InsertVarInitStmtsProcessor() {
	// TODO Auto-generated constructor stub

}

InsertVarInitStmtsProcessor::~InsertVarInitStmtsProcessor() {
	// TODO Auto-generated destructor stub
}

bool InsertVarInitStmtsProcessor::process(IModel *m) {
	visit_model(m);

	return true;
}

void InsertVarInitStmtsProcessor::visit_action(IAction *a) {
	ExecImpl *pre_solve = 0;

	for (std::vector<IBaseItem *>::const_iterator it=a->getItems().begin();
			it!=a->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeExec) {
			ExecImpl *e = dynamic_cast<ExecImpl *>(*it);
			if (e->getExecKind() == IExec::PreSolve) {
				pre_solve = e;
				break;
			}
		}
	}

	if (!pre_solve) {
		// Need to create and insert one
		std::vector<IExecStmt *> stmts;
		pre_solve = new ExecImpl(IExec::PreSolve, stmts);

		dynamic_cast<ScopeItemImpl *>(a)->insert(0, pre_solve);
	}

	// Insert temp-var initialization statements
	for (std::vector<IBaseItem *>::const_iterator it=a->getItems().begin();
			it!=a->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeVendor) {
			RulesItem *rit = dynamic_cast<RulesItem *>(*it);
			if (rit->getItemType() == RulesItem::RulesItem_TempVar) {
				RulesTempVar *var = dynamic_cast<RulesTempVar *>(rit);
				std::vector<IField *> path;
				path.push_back(var);

				ExecExprStmtImpl *init_e = new ExecExprStmtImpl(
						new FieldRefImpl(path),
						IExecExprStmt::AssignOp_Eq,
						new LiteralImpl((int64_t)0));
				pre_solve->getStmtsM().insert(
					pre_solve->getStmtsM().begin(), 
					init_e);
			}
		}
	}

	RulesPSIVisitor::visit_action(a);
}

void InsertVarInitStmtsProcessor::visit_struct(IStruct *s) {
	ExecImpl *pre_solve = 0;

	for (std::vector<IBaseItem *>::const_iterator it=s->getItems().begin();
			it!=s->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeExec) {
			ExecImpl *e = dynamic_cast<ExecImpl *>(*it);
			if (e->getExecKind() == IExec::PreSolve) {
				pre_solve = e;
				break;
			}
		}
	}

	if (!pre_solve) {
		// Need to create and insert one
		std::vector<IExecStmt *> stmts;
		pre_solve = new ExecImpl(IExec::PreSolve, stmts);

		dynamic_cast<StructImpl *>(s)->insert(0, pre_solve);
	}

	RulesPSIVisitor::visit_struct(s);
}

} /* namespace qpssc */
