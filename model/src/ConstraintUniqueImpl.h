/*
 * ConstraintUniqueImpl.h
 *
 *  Created on: Mar 31, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_
#define MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_

#include "IConstraintUnique.h"
#include "BaseItemImpl.h"

using namespace psi;

class ConstraintUniqueImpl: public virtual IConstraintUnique, public virtual BaseItemImpl {
public:

	ConstraintUniqueImpl(IOpenRangeList	*target);

	virtual ~ConstraintUniqueImpl();

	virtual ConstraintType getConstraintType() const {
		return IConstraint::ConstraintType_Unique;
	}

	virtual IOpenRangeList *getTarget() const { return m_target; }

	virtual IBaseItem *clone() const;

private:
	IOpenRangeList					*m_target;
};

#endif /* MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_ */
