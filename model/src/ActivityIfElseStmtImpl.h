/*
 * ActivityIfElseStmtImpl.h
 *
 *  Created on: Apr 7, 2018
 *      Author: ballance
 */

#pragma once

#include "BaseItemImpl.h"
#include "IActivityIfElseStmt.h"

using namespace psi;

class ActivityIfElseStmtImpl:
		public virtual BaseItemImpl,
		public virtual IActivityIfElseStmt {
public:
	ActivityIfElseStmtImpl(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt);

	virtual ~ActivityIfElseStmtImpl();

	virtual ActivityStmtType getStmtType() const {
		return IActivityStmt::ActivityStmt_IfElse;
	}

	virtual IExpr *getCond() const { return m_cond; }

	virtual IActivityStmt *getTrue() const { return m_true; }

	virtual IActivityStmt *getFalse() const { return m_false; }

	virtual IActivityStmt *clone() const;

private:
	IExpr				*m_cond;
	IActivityStmt		*m_true;
	IActivityStmt		*m_false;
};

