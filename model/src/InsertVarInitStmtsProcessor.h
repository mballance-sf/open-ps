/*
 * InsertVarInitStmtsProcessor.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_INSERTVARINITSTMTSPROCESSOR_H_
#define SRC_INSERTVARINITSTMTSPROCESSOR_H_

#include "IModelProcessor.h"
#include "RulesPSIVisitor.h"

namespace qpssc {

class InsertVarInitStmtsProcessor:
		public RulesPSIVisitor,
		public virtual IModelProcessor {

public:
	InsertVarInitStmtsProcessor();

	virtual ~InsertVarInitStmtsProcessor();

	virtual bool process(IModel *model);

	virtual void visit_action(IAction *a);

	virtual void visit_struct(IStruct *s);

};

} /* namespace qpssc */

#endif /* SRC_INSERTVARINITSTMTSPROCESSOR_H_ */
