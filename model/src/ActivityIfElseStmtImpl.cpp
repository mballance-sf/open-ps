/*
 * ActivityIfElseStmtImpl.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: ballance
 */

#include "ActivityIfElseStmtImpl.h"

ActivityIfElseStmtImpl::ActivityIfElseStmtImpl(
		IExpr				*cond,
		IActivityStmt		*true_stmt,
		IActivityStmt		*false_stmt) :
		BaseItemImpl(IBaseItem::TypeActivityStmt),
		m_cond(cond), m_true(true_stmt), m_false(false_stmt) {

}

ActivityIfElseStmtImpl::~ActivityIfElseStmtImpl() {
	// TODO Auto-generated destructor stub
}

IActivityStmt *ActivityIfElseStmtImpl::clone() const {
	return new ActivityIfElseStmtImpl(m_cond, m_true, m_false);
}


