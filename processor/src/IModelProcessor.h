/*
 * IModelProcessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#ifndef SRC_IMODELPROCESSOR_H_
#define SRC_IMODELPROCESSOR_H_

#include "IModel.h"

using namespace psi_api;

namespace qpssc {

class IModelProcessor {
public:

	virtual ~IModelProcessor() { }

	virtual bool process(psi_api::IModel *model) = 0;

	// TODO: error handling
};

}




#endif /* SRC_IMODELPROCESSOR_H_ */
