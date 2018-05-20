/*
 * ActivityDoActionStmtImpl.h
 *
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ACTIVITYDOACTIONSTMTIMPL_H_
#define MODEL_SRC_ACTIVITYDOACTIONSTMTIMPL_H_
#include "BaseItemImpl.h"
#include "IActivityDoActionStmt.h"



class ActivityDoActionStmtImpl: public virtual BaseItemImpl, public virtual IActivityDoActionStmt {
public:
	ActivityDoActionStmtImpl(
			IBaseItem			*target,
			IConstraintBlock	*constraint);

	virtual ~ActivityDoActionStmtImpl();

	virtual IBaseItem *getTargetType() const { return m_target; }

	virtual IConstraintBlock *getConstraint() const {
		return m_constraint;
	}

	virtual ActivityStmtType getStmtType() const {
		return ActivityStmt_DoAction;
	}

	virtual IActivityStmt *clone() const;

private:
	IBaseItem					*m_target;
	IConstraintBlock			*m_constraint;

};

#endif /* MODEL_SRC_ACTIVITYDOACTIONSTMTIMPL_H_ */
