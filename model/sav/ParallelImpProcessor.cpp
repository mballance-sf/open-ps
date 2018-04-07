/*
 * ParallelImpProcessor.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: ballance
 */

#include "ParallelImpProcessor.h"

#include "../src/ActivityTraverseStmtImpl.h"
#include "MethodCallExprImpl.h"
#include "ExecExprStmtImpl.h"
#include "ActionImpl.h"
#include "ActivityBlockStmtImpl.h"
#include "FieldRefImpl.h"

namespace qpssc {

ParallelImpProcessor::ParallelImpProcessor() {
	// TODO Auto-generated constructor stub

}

ParallelImpProcessor::~ParallelImpProcessor() {
	// TODO Auto-generated destructor stub
}

bool ParallelImpProcessor::process(IModel *model) {

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
			if (f->getName() == "thread_context_create") {
				m_thread_context_create = f;
			} else if (f->getName() == "thread_context_join") {
				m_thread_context_join = f;
			} else if (f->getName() == "thread_begin") {
				m_thread_begin = f;
			} else if (f->getName() == "thread_end") {
				m_thread_end = f;
			}
		}
	}

	RulesPSIVisitor::visit_model(model);

	return true;
}

void ParallelImpProcessor::visit_action(IAction *a) {
	// Ensure we can detect that we need to create these actions
	m_thread_begin_a = 0;
	m_thread_end_a = 0;
	m_thread_context_create_a = 0;
	m_thread_context_join_a = 0;

	std::vector<IField *> path;

	RulesPSIVisitor::visit_action(a);

	// Perform post-traversal insertion
	for (uint32_t i=0; i<m_parallel_ins_p.size(); i++) {
		ActivityBlockStmtImpl *block = dynamic_cast<ActivityBlockStmtImpl *>(
				m_parallel_ins_p.at(i));
		std::vector<IActivityStmt *>::iterator pos = m_parallel_ins.at(i);
		IActivityStmt *s = *pos;

		path.clear();
		path.push_back(m_thread_context_create_a);
		block->getStmtsM().insert(pos,
			new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));

//		pos++;
//		if (pos != block->getStmtsM().end()) {
//			pos++;
//		}

		// Spin back through the list to find the point *after*
		for (pos=block->getStmtsM().begin(); pos!=block->getStmtsM().end(); pos++) {
			if (*pos == s) {
				pos++;
				break;
			}
		}

		path.clear();
		path.push_back(m_thread_context_join_a);
		if (pos == block->getStmtsM().end()) {
			block->getStmtsM().push_back(
					new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));
		} else {
			block->getStmtsM().insert(pos,
					new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));
		}
	}

	m_parallel_ins_p.clear();
	m_parallel_ins.clear();
}

void ParallelImpProcessor::visit_graph_parallel_block_stmt(IActivityBlockStmt *s) {

	ActionImpl *a = dynamic_cast<ActionImpl *>(scope_parent());

	if (!m_thread_begin_a) {
		std::vector<IExpr *> params;
		// Create the thread-control actions
		m_thread_begin_a = new RulesExecActionItem("__thread_begin");
		m_thread_begin_a->getExec()->getStmtsM().push_back(
				new ExecExprStmtImpl(
						new MethodCallExprImpl(m_thread_begin, params),
						IExecExprStmt::AssignOp_None, 0));
		a->add(m_thread_begin_a);
		m_thread_end_a = new RulesExecActionItem("__thread_end");
		m_thread_end_a->getExec()->getStmtsM().push_back(
				new ExecExprStmtImpl(
						new MethodCallExprImpl(m_thread_end, params),
						IExecExprStmt::AssignOp_None, 0));
		a->add(m_thread_end_a);
		m_thread_context_create_a = new RulesExecActionItem("__thread_context_create");
		m_thread_context_create_a->getExec()->getStmtsM().push_back(
				new ExecExprStmtImpl(
						new MethodCallExprImpl(m_thread_context_create, params),
						IExecExprStmt::AssignOp_None, 0));
		a->add(m_thread_context_create_a);
		m_thread_context_join_a = new RulesExecActionItem("__thread_context_join");
		m_thread_context_join_a->getExec()->getStmtsM().push_back(
				new ExecExprStmtImpl(
						new MethodCallExprImpl(m_thread_context_join, params),
						IExecExprStmt::AssignOp_None, 0));
		a->add(m_thread_context_join_a);
	}

	// Stitch this into the parallel flow
	IActivityStmt *gp = graph_parent(s);
	ActivityBlockStmtImpl *gp_block = dynamic_cast<ActivityBlockStmtImpl *>(gp);

	if (!gp_block) {
		fprintf(stdout, "Fatal: Not a block\n");
	}

	std::vector<IActivityStmt *>::iterator parallel_pos;
	for (parallel_pos=gp_block->getStmtsM().begin();
			parallel_pos!=gp_block->getStmtsM().end(); parallel_pos++) {
		if (*parallel_pos == s) {
			break;
		}
	}

	if (parallel_pos == gp_block->getStmtsM().end()) {
		fprintf(stdout, "Fatal: Failed to find parallel in parent scope\n");
	}

	m_parallel_ins_p.push_back(gp_block);
	m_parallel_ins.push_back(parallel_pos);

	std::vector<IField *> path;

	// Insert thread_begin/thread_end in the children of the parallel block
	for (std::vector<IActivityStmt *>::const_iterator it=s->getStmts().begin();
			it!=s->getStmts().end(); it++) {
		ActivityBlockStmtImpl *block = dynamic_cast<ActivityBlockStmtImpl *>(*it);
		if (!block) {
			fprintf(stdout, "Error: Fatal - parallel branch is not a block\n");
		}
		path.clear();
		path.push_back(m_thread_begin_a);
		block->getStmtsM().insert(
				block->getStmtsM().begin(),
				new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));

		path.clear();
		path.push_back(m_thread_end_a);
		block->getStmtsM().push_back(
				new ActivityTraverseStmtImpl(new FieldRefImpl(path), 0));
	}


	// Handle nested parallel blocks
	RulesPSIVisitor::visit_graph_parallel_block_stmt(s);
}

} /* namespace qpssc */
