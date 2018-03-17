/*
 * IVariableRef.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IVARIABLEREF_H_
#define MODEL_SRC_IVARIABLEREF_H_
#include "IExpr.h"
#include "IScopeItem.h"
#include <string>

using namespace psi_api;

class IVariableRef : public virtual IExpr {
public:

	virtual ~IVariableRef() { }

	virtual IBaseItem *getScope() const = 0;

	virtual const std::string &getId() const = 0;

	virtual IExpr *getIndexLhs() const = 0;

	virtual IExpr *getIndexRhs() const = 0;

	virtual void setNext(IVariableRef *next) = 0;

	// Returns the next link in the variable-ref chain
	virtual IVariableRef *getNext() const = 0;

	virtual std::string toString() const = 0;

};




#endif /* MODEL_SRC_IVARIABLEREF_H_ */
