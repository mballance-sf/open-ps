/*
 * CoverspecImpl.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#include "CoverspecImpl.h"

CoverspecImpl::CoverspecImpl(const std::string &name) :
	BaseItemImpl(IBaseItem::TypeCoverspec), m_name(name) {

}

CoverspecImpl::~CoverspecImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *CoverspecImpl::clone() const {
	CoverspecImpl *ret = new CoverspecImpl(m_name);

	for (std::vector<IBaseItem *>::const_iterator it=getItems().begin();
			it!=getItems().end(); it++) {
		ret->add((*it)->clone());
	}

	return ret;
}

