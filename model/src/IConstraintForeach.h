/*
 * IConstraintForeach.h
 *
 *  Created on: Mar 28, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ICONSTRAINTFOREACH_H_
#define MODEL_SRC_ICONSTRAINTFOREACH_H_
#include "IConstraint.h"
#include "IConstraintBlock.h"
#include "IVariableRef.h"

using namespace psi_api;

class IConstraintForeach : public IConstraint {
public:

	virtual ~IConstraintForeach() { }

	virtual IVariableRef *getTarget() const = 0;

	virtual const std::string &getIteratorName() const = 0;

	virtual IConstraintBlock *getBody() const = 0;

};




#endif /* MODEL_SRC_ICONSTRAINTFOREACH_H_ */
