/*
 * RepeatImpProcessor.h
 *
 *  Created on: Nov 1, 2016
 *      Author: ballance
 */

#ifndef SRC_REPEATIMPPROCESSOR_H_
#define SRC_REPEATIMPPROCESSOR_H_

#include "RulesPSIVisitor.h"
#include "IModelProcessor.h"

namespace qpssc {

class RepeatImpProcessor: public RulesPSIVisitor, public virtual IModelProcessor {
public:

	RepeatImpProcessor();

	virtual ~RepeatImpProcessor();

	virtual bool process(psi_api::IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_graph_repeat_stmt(IActivityRepeatStmt *repeat);


private:

	uint32_t				m_repeat_id;
	IImportFunc				*m_stop_loop_expansion;
	IImportFunc				*m_repeat_begin;
	IImportFunc				*m_repeat_end;

};

} /* namespace qpssc */

#endif /* SRC_REPEATIMPPROCESSOR_H_ */
