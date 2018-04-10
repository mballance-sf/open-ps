/*
 * FlattenParallelProcessor.h
 *
 *  Created on: Jan 26, 2017
 *      Author: ballance
 */

#ifndef SRC_FLATTENPARALLELPROCESSOR_H_
#define SRC_FLATTENPARALLELPROCESSOR_H_

#include "IModelProcessor.h"
#include "RulesPSIVisitor.h"

namespace qpssc {

class FlattenParallelProcessor:
		public virtual RulesPSIVisitor,
		public virtual IModelProcessor {
public:

	FlattenParallelProcessor();

	virtual ~FlattenParallelProcessor();

	virtual bool process(IModel *model);

	virtual void visit_activity_block_stmt(IActivityBlockStmt *block);

};

} /* namespace qpssc */

#endif /* SRC_FLATTENPARALLELPROCESSOR_H_ */
