/*
 * InExprImpl.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: ballance
 */

#include "InExprImpl.h"

InExprImpl::InExprImpl(IExpr *lhs, IOpenRangeList *rhs) :
	ExprImpl(ExprType_In), m_lhs(lhs), m_rhs(rhs) {

}

InExprImpl::~InExprImpl() {
	// TODO Auto-generated destructor stub
}

IExpr *InExprImpl::clone() const {
	return new InExprImpl(m_lhs, m_rhs);
}
