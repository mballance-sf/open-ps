/*
 * ParallelImpProcessor.h
 *
 *  Created on: Jan 27, 2017
 *      Author: ballance
 */

#ifndef SRC_PARALLELIMPPROCESSOR_H_
#define SRC_PARALLELIMPPROCESSOR_H_

#include "IModelProcessor.h"
#include "RulesPSIVisitor.h"
#include "RulesExecActionItem.h"

namespace qpssc {

class ParallelImpProcessor:
		public virtual IModelProcessor,
		public virtual RulesPSIVisitor {
public:

	ParallelImpProcessor();

	virtual ~ParallelImpProcessor();

	virtual bool process(IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_graph_parallel_block_stmt(IActivityBlockStmt *s);

private:

	IImportFunc				*m_thread_context_create;
	IImportFunc				*m_thread_context_join;
	IImportFunc				*m_thread_begin;
	IImportFunc				*m_thread_end;

	RulesExecActionItem		*m_thread_context_create_a;
	RulesExecActionItem		*m_thread_context_join_a;
	RulesExecActionItem		*m_thread_begin_a;
	RulesExecActionItem		*m_thread_end_a;

	std::vector<IActivityBlockStmt *>						m_parallel_ins_p;
	std::vector<std::vector<IActivityStmt *>::iterator>	m_parallel_ins;
};

} /* namespace qpssc */

#endif /* SRC_PARALLELIMPPROCESSOR_H_ */
