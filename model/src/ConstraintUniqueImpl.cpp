/*
 * ConstraintUniqueImpl.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: ballance
 */

#include "ConstraintUniqueImpl.h"

ConstraintUniqueImpl::ConstraintUniqueImpl(
		const std::vector<IExpr *>		&terms) :
	BaseItemImpl(IBaseItem::TypeConstraint), m_terms(terms) {
	// TODO Auto-generated constructor stub

}

ConstraintUniqueImpl::~ConstraintUniqueImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ConstraintUniqueImpl::clone() const {
	return new ConstraintUniqueImpl(m_terms);
}
