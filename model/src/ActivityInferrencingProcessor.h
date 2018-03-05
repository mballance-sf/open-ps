/*
 * ActivityInferrencingProcessor.h
 *
 *  Created on: Jan 20, 2017
 *      Author: ballance
 */

#ifndef SRC_ACTIVITYINFERRENCINGPROCESSOR_H_
#define SRC_ACTIVITYINFERRENCINGPROCESSOR_H_

#include "IModelProcessor.h"
#include "RulesPSIVisitor.h"

namespace qpssc {

class ActivityInferrencingProcessor:
		public IModelProcessor,
		public virtual RulesPSIVisitor {
public:
	ActivityInferrencingProcessor();

	virtual ~ActivityInferrencingProcessor();

	virtual bool process(IModel *model);
};

} /* namespace qpssc */

#endif /* SRC_ACTIVITYINFERRENCINGPROCESSOR_H_ */
