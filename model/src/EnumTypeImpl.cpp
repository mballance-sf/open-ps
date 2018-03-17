/*
 * EnumTypeImpl.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: ballance
 */

#include "EnumTypeImpl.h"

EnumTypeImpl::EnumTypeImpl(
		const std::string					&name,
		const std::vector<IEnumerator *>	&enumerators) :
			BaseItemImpl(IBaseItem::TypeEnum),
			m_name(name), m_enumerators(enumerators) {
	// TODO Auto-generated constructor stub

}

EnumTypeImpl::~EnumTypeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *EnumTypeImpl::clone() const {
	return new EnumTypeImpl(m_name, m_enumerators);
}

