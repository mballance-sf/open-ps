/*
 * RepeatImpProcessor.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: ballance
 */

#include "RepeatImpProcessor.h"
#include "ActivityBlockStmtImpl.h"
#include "ActivityRepeatStmtImpl.h"
#include "RulesExecActionItem.h"
#include "ExecExprStmtImpl.h"
#include "RulesTempVar.h"
#include "FieldRefImpl.h"
#include "ActionImpl.h"
#include "BinaryExprImpl.h"
#include "LiteralImpl.h"
#include <stdio.h>
#include "../../psi/src/MethodCallExprImpl.h"
#include "../src/ActivityTraverseStmtImpl.h"

namespace qpssc {

RepeatImpProcessor::RepeatImpProcessor() {
	// TODO Auto-generated constructor stub

}

RepeatImpProcessor::~RepeatImpProcessor() {
	// TODO Auto-generated destructor stub
}

bool RepeatImpProcessor::process(psi_api::IModel *model) {
	// First, find the 'stop_loop_expansion' builtin
	IPackage *builtins = 0;
	for (std::vector<IBaseItem *>::const_iterator it=model->getItems().begin();
			it!=model->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypePackage &&
				dynamic_cast<IPackage *>(*it)->getName() == "inFact") {
			builtins = dynamic_cast<IPackage *>(*it);
			break;
		}
	}

	for (std::vector<IBaseItem *>::const_iterator it=builtins->getItems().begin();
			it!=builtins->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeImportFunc) {
			IImportFunc *f = dynamic_cast<IImportFunc *>(*it);
			if (f->getName() == "stop_loop_expansion") {
				m_stop_loop_expansion = f;
			} else if (f->getName() == "repeat_begin") {
				m_repeat_begin = f;
			} else if (f->getName() == "repeat_end") {
				m_repeat_end = f;
			}
		}
	}

	RulesPSIVisitor::visit_model(model);

	return true;
}

void RepeatImpProcessor::visit_action(IAction *a) {
	m_repeat_id = 1;
	RulesPSIVisitor::visit_action(a);
}

void RepeatImpProcessor::visit_graph_repeat_stmt(IActivityRepeatStmt *r) {
	char tmp[128];
	ActivityBlockStmtImpl *p;

	// The action containing this graph
	ActionImpl *a = dynamic_cast<ActionImpl *>(scope_parent());

	if (graph_parent(r)) {
		p = dynamic_cast<ActivityBlockStmtImpl *>(graph_parent(r));
	} else {
		// Need to convert this to a block
		p = new ActivityBlockStmtImpl(IActivityStmt::ActivityStmt_Block);
		p->add(r);

		a->setGraph(p);
	}

	// Need to ensure that the repeat body is a block
	if (r->getBody()->getStmtType() != IActivityStmt::ActivityStmt_Block) {
		ActivityBlockStmtImpl *b = new ActivityBlockStmtImpl(IActivityStmt::ActivityStmt_Block);
		b->add(r->getBody());
		dynamic_cast<ActivityRepeatStmtImpl *>(r)->setBody(b);
	}
	ActivityBlockStmtImpl *rb = dynamic_cast<ActivityBlockStmtImpl *>(r->getBody());

	// Create temp vars
	sprintf(tmp, "_repeat_%d_index", m_repeat_id);
	RulesTempVar *repeat_index = new RulesTempVar(tmp);
	a->add(repeat_index);

	sprintf(tmp, "_repeat_%d_limit", m_repeat_id);
	RulesTempVar *repeat_limit = new RulesTempVar(tmp);
	a->add(repeat_limit);

	sprintf(tmp, "_repeat_%d_init", m_repeat_id);
	RulesTempVar *repeat_init = new RulesTempVar(tmp);
	a->add(repeat_init);

	// Create new actions to represent the init and check_cond
	sprintf(tmp, "_repeat_init_%d", m_repeat_id);
	RulesExecActionItem *init = new RulesExecActionItem(tmp);

	// Specify the body
	std::vector<IExecStmt *> stmts;
	init->getExec()->getStmtsM().push_back(
			new RulesExecStmtIfItem(
					new BinaryExprImpl( // init == 0
							new FieldRefImpl(repeat_init),
							IBinaryExpr::BinOp_EqEq,
							new LiteralImpl((uint64_t)0)),
					{
							new ExecExprStmtImpl(
									new FieldRefImpl(repeat_index),
									IExecExprStmt::AssignOp_Eq,
									new LiteralImpl((uint64_t)0)),
							new ExecExprStmtImpl(
									new FieldRefImpl(repeat_limit),
									IExecExprStmt::AssignOp_Eq,
									r->getCond()),
							new ExecExprStmtImpl(
									new FieldRefImpl(repeat_init),
									IExecExprStmt::AssignOp_Eq,
									new LiteralImpl((uint64_t)1)),
							new ExecExprStmtImpl(
									new MethodCallExprImpl(m_repeat_begin, {}))
					},
					{}
			));


	a->add(init);

	sprintf(tmp, "_repeat_check_%d", m_repeat_id);
	RulesExecActionItem *check = new RulesExecActionItem(tmp);
	std::vector<IExpr *> parameters;
	check->getExec()->getStmtsM().push_back(
			new ExecExprStmtImpl(
					new FieldRefImpl(repeat_index),
					IExecExprStmt::AssignOp_PlusEq,
					new LiteralImpl((uint64_t)1))
	);
	check->getExec()->getStmtsM().push_back(
			new RulesExecStmtIfItem(
					new BinaryExprImpl(
							new FieldRefImpl(repeat_index),
							IBinaryExpr::BinOp_GE,
							new FieldRefImpl(repeat_limit)
					),
				{
						new ExecExprStmtImpl(
								new MethodCallExprImpl(m_repeat_end, {})),
						new ExecExprStmtImpl(
								new MethodCallExprImpl(m_stop_loop_expansion, {})),
						new ExecExprStmtImpl(
								new FieldRefImpl(repeat_init),
								IExecExprStmt::AssignOp_Eq,
								new LiteralImpl((uint64_t)0))
				}, {})
	);
	a->add(check);

	// Now, need to traverse these right before the repeat
	// and at the end of the repeat
	std::vector<IField *> path;

	path.clear();
	path.push_back(init);
	rb->getStmtsM().insert(
		rb->getStmtsM().begin(), 
		new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));

	path.clear();
	path.push_back(check);
	rb->add(new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));

	m_repeat_id++;

	// Handles nested repeats
	RulesPSIVisitor::visit_graph_repeat_stmt(r);
}

} /* namespace qpssc */
