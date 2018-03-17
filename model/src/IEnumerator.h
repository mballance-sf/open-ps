/*
 * IEnumerator.h
 *
 *  Created on: Mar 15, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IENUMERATOR_H_
#define MODEL_SRC_IENUMERATOR_H_
#include "IBaseItem.h"
#include "INamedItem.h"
#include "IExpr.h"

using namespace psi_api;


class IEnumerator : public virtual INamedItem {
public:

	virtual const std::string &getName() const = 0;

	virtual IExpr *getValue() const = 0;

};



#endif /* MODEL_SRC_IENUMERATOR_H_ */
