/*
 * ExtendCompositeImpl.cpp
 *
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#include "ExtendCompositeImpl.h"

ExtendCompositeImpl::ExtendCompositeImpl(
		IExtend::ExtendType		type,
		IBaseItem				*target) :
		BaseItemImpl(IBaseItem::TypeExtend),
		m_extend_type(type),
		m_target(target) {

}

ExtendCompositeImpl::~ExtendCompositeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ExtendCompositeImpl::clone() const {
	ExtendCompositeImpl *ret = new ExtendCompositeImpl(getExtendType(), getTarget());

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

