/*
 * ActivitySelectBranchStmtImpl.cpp
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#include "ActivitySelectBranchStmtImpl.h"

ActivitySelectBranchStmtImpl::ActivitySelectBranchStmtImpl(
		IActivityStmt			*stmt,
		IExpr					*guard,
		IExpr					*weight) {
	m_stmt = stmt;
	m_guard = guard;
	m_weight = weight;
}

ActivitySelectBranchStmtImpl::~ActivitySelectBranchStmtImpl() {
	// TODO Auto-generated destructor stub
}

