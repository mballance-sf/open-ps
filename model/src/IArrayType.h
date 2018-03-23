/*
 * IArrayType.h
 *
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IARRAYTYPE_H_
#define MODEL_SRC_IARRAYTYPE_H_
#include "IBaseItem.h"
#include "IScopeItem.h"
#include "IField.h"
#include "IExpr.h"

using namespace psi_api;

class IArrayType : public virtual IBaseItem, public virtual IScopeItem {
public:

	virtual ~IArrayType() { }

	virtual bool hasSum() const = 0;

	virtual IField *getSize() const = 0;

	virtual IField *getSum() const = 0;

	virtual IBaseItem *getTargetType() const = 0;

	virtual IExpr *getLhs() const = 0;

	virtual IExpr *getRhs() const = 0;

};




#endif /* MODEL_SRC_IARRAYTYPE_H_ */
