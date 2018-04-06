/*
 * ConstraintUniqueImpl.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: ballance
 */

#include "ConstraintUniqueImpl.h"

ConstraintUniqueImpl::ConstraintUniqueImpl(IOpenRangeList *target) :
	BaseItemImpl(IBaseItem::TypeConstraint), m_target(target) {

}

ConstraintUniqueImpl::~ConstraintUniqueImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ConstraintUniqueImpl::clone() const {
	return new ConstraintUniqueImpl(m_target);
}
