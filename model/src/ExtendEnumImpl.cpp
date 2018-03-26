/*
 * ExtendEnumImpl.cpp
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#include "ExtendEnumImpl.h"

ExtendEnumImpl::ExtendEnumImpl(
		IBaseItem							*target,
		const std::vector<IEnumerator *>	&enumerators
		) : BaseItemImpl(IBaseItem::TypeExtend),
		m_target(target), m_enumerators(enumerators) {

}

ExtendEnumImpl::~ExtendEnumImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ExtendEnumImpl::clone() const {
	return new ExtendEnumImpl(m_target, m_enumerators);
}
