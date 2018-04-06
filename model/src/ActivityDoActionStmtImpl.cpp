/*
 * ActivityDoActionStmtImpl.cpp
 *
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#include "ActivityDoActionStmtImpl.h"

ActivityDoActionStmtImpl::ActivityDoActionStmtImpl(
		IBaseItem			*target,
		IConstraintBlock	*constraint) :
		m_target(target), m_constraint(constraint) {

}

ActivityDoActionStmtImpl::~ActivityDoActionStmtImpl() {
	// TODO Auto-generated destructor stub
}

IGraphStmt *ActivityDoActionStmtImpl::clone() const {
	return new ActivityDoActionStmtImpl(m_target, m_constraint);
}