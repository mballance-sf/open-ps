/*
 * ActivitySelectStmtImpl.h
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#pragma once
#include "BaseItemImpl.h"
#include "IActivitySelectStmt.h"

class ActivitySelectStmtImpl : public virtual BaseItemImpl, public virtual IActivitySelectStmt {
public:
	ActivitySelectStmtImpl(
			const std::vector<IActivitySelectBranchStmt *> &branches);

	virtual ~ActivitySelectStmtImpl();

	virtual IActivityStmt::ActivityStmtType getStmtType() const {
		return IActivityStmt::ActivityStmt_Select;
	}

	virtual const std::vector<IActivitySelectBranchStmt *> &getBranches() const {
		return m_branches;
	}

	virtual IActivityStmt *clone() const;

private:
	std::vector<IActivitySelectBranchStmt *>		m_branches;
};

