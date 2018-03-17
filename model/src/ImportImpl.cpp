/*
 * ImportImpl.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#include "ImportImpl.h"

ImportImpl::ImportImpl(IBaseItem *target, bool is_wildcard) :
	BaseItemImpl(IBaseItem::TypeImport),
	m_target(target), m_is_wildcard(is_wildcard) {

}

ImportImpl::~ImportImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ImportImpl::clone() const {
	return new ImportImpl(m_target, m_is_wildcard);
}

