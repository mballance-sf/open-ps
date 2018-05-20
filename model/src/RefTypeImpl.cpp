/*
 * RefTypeImpl.cpp
 *
 *  Created on: Mar 9, 2018
 *      Author: ballance
 */

#include "RefTypeImpl.h"

RefTypeImpl::RefTypeImpl(
		IScopeItem						*scope,
		const std::vector<std::string>	&path) :
		BaseItemImpl(IBaseItem::TypeRefType),
		m_scope(scope), m_typeid(path), m_target(0) {

}

RefTypeImpl::~RefTypeImpl() {
	// TODO Auto-generated destructor stub
}

IScopeItem *RefTypeImpl::getScope() const {
	return m_scope;
}

const std::vector<std::string> &RefTypeImpl::getTypeId() const {
	return m_typeid;
}

IBaseItem *RefTypeImpl::getTargetType() const {
	return m_target;
}

void RefTypeImpl::setTargetType(IBaseItem *t) {
	m_target = t;
}

IBaseItem *RefTypeImpl::clone() const {
	// TODO:
	return 0;
}

std::string RefTypeImpl::toString() const {
	std::string ret;
	for (uint32_t i=0; i<m_typeid.size(); i++) {
		ret += m_typeid.at(i);
		if (i+1 < m_typeid.size()) {
			ret += "::";
		}
	}

	return ret;
}
