/*
 * ConstraintForeachImpl.h
 *
 *  Created on: Mar 28, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_CONSTRAINTFOREACHIMPL_H_
#define MODEL_SRC_CONSTRAINTFOREACHIMPL_H_

#include "IConstraintForeach.h"
#include "BaseItemImpl.h"



class ConstraintForeachImpl:
		public virtual IConstraintForeach,
		public virtual BaseItemImpl {
public:
	ConstraintForeachImpl(
			IVariableRef			*target,
			const std::string		&iterator,
			IConstraintBlock		*body);

	virtual ~ConstraintForeachImpl();

	virtual ConstraintType getConstraintType() const { return IConstraint::ConstraintType_Foreach; }

	virtual IVariableRef *getTarget() const { return m_target; }

	virtual const std::string &getIteratorName() const { return m_iterator; }

	virtual IConstraintBlock *getBody() const { return m_body; }

	virtual IBaseItem *clone() const;

private:
	IVariableRef					*m_target;
	std::string						m_iterator;
	IConstraintBlock				*m_body;

};

#endif /* MODEL_SRC_CONSTRAINTFOREACHIMPL_H_ */
