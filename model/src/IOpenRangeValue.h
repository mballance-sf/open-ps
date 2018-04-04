/*
 * IOpenRangeValue.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IOPENRANGEVALUE_H_
#define MODEL_SRC_IOPENRANGEVALUE_H_
#include "IExpr.h"

using namespace psi_api;

class IOpenRangeValue {
public:

	virtual ~IOpenRangeValue() { }

	// ..<expr> || <expr>..
	virtual bool isDomainBound() const = 0;

	virtual IExpr *getLHS() const = 0;

	virtual IExpr *getRHS() const = 0;

};





#endif /* MODEL_SRC_IOPENRANGEVALUE_H_ */
