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

	ConstraintUniqueImpl(
			const std::vector<IExpr *>	&terms);

	virtual ~ConstraintUniqueImpl();

	virtual ConstraintType getConstraintType() const {
		return IConstraint::ConstraintType_Unique;
	}

	virtual const std::vector<IExpr *> &getTerms() const { return m_terms; }

	virtual IBaseItem *clone() const;

private:
	std::vector<IExpr *>			m_terms;
};

#endif /* MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_ */
