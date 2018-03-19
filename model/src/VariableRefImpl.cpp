/*
 * VariableRefImpl.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#include "VariableRefImpl.h"

VariableRefImpl::VariableRefImpl(
		IBaseItem				*scope,
		const std::string		&id,
		IExpr					*index_lhs,
		IExpr					*index_rhs) :
		ExprImpl(IExpr::ExprType_VariableRef),
		m_scope(scope), m_target(0), m_id(id),
		m_index_lhs(index_lhs), m_index_rhs(index_rhs),
		m_next(0) {

}

VariableRefImpl::~VariableRefImpl() {
	// TODO Auto-generated destructor stub
}

std::string VariableRefImpl::toString() const {
	std::string ret = m_id;

	if (m_next) {
		ret += ".";
		ret += m_next->toString();
	}

	return ret;
}

IExpr *VariableRefImpl::clone() const {
	VariableRefImpl *ret = new VariableRefImpl(m_scope, m_id, m_index_lhs, m_index_rhs);
	ret->setNext(getNext());

	return ret;
}
