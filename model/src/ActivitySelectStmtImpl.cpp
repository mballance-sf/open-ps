/*
 * ActivitySelectStmtImpl.cpp
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#include "ActivitySelectStmtImpl.h"

ActivitySelectStmtImpl::ActivitySelectStmtImpl(
			const std::vector<IActivitySelectBranchStmt *> &branches) :
			BaseItemImpl(IBaseItem::TypeActivityStmt), m_branches(branches) {

}

ActivitySelectStmtImpl::~ActivitySelectStmtImpl() {
	// TODO Auto-generated destructor stub
}

IActivityStmt *ActivitySelectStmtImpl::clone() const {
	return new ActivitySelectStmtImpl(m_branches);
}

