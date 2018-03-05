/*
 * CreateBuiltinFuncsProcessor.h
 *
 *  Created on: Nov 3, 2016
 *      Author: ballance
 */

#ifndef SRC_CREATEBUILTINFUNCSPROCESSOR_H_
#define SRC_CREATEBUILTINFUNCSPROCESSOR_H_
#include "IModelProcessor.h"

namespace qpssc {

class CreateBuiltinFuncsProcessor : public virtual IModelProcessor {
public:
	CreateBuiltinFuncsProcessor();

	virtual ~CreateBuiltinFuncsProcessor();

	virtual bool process(IModel *model);

};

} /* namespace qpssc */

#endif /* SRC_CREATEBUILTINFUNCSPROCESSOR_H_ */
