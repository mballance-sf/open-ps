/*
 * ActivitySelectBranchStmtImpl.h
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#pragma once
#include "IActivitySelectBranchStmt.h"

class ActivitySelectBranchStmtImpl: public IActivitySelectBranchStmt {
public:

	ActivitySelectBranchStmtImpl(
			IActivityStmt	*stmt,
			IExpr			*guard,
			IExpr			*weight);

	virtual ~ActivitySelectBranchStmtImpl();

	virtual IExpr *getGuard() const { return m_guard; }

	virtual IExpr *getWeight() const { return m_weight; }

	virtual IActivityStmt *getStmt() const { return m_stmt; }

private:

	IActivityStmt				*m_stmt;
	IExpr						*m_guard;
	IExpr						*m_weight;

};

