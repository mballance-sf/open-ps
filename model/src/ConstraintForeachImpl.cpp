/*
 * ConstraintForeachImpl.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: ballance
 */

#include "ConstraintForeachImpl.h"

ConstraintForeachImpl::ConstraintForeachImpl(
		IVariableRef			*target,
		const std::string		&iterator,
		IConstraintBlock		*body) : BaseItemImpl(IBaseItem::TypeConstraint) {
	m_target = target;
	m_iterator = iterator;
	m_body = body;
}

ConstraintForeachImpl::~ConstraintForeachImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ConstraintForeachImpl::clone() const {
	return new ConstraintForeachImpl(
			m_target,
			m_iterator,
			m_body);
}
