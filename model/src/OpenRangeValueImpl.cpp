/*
 * OpenRangeValueImpl.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: ballance
 */

#include "OpenRangeValueImpl.h"

OpenRangeValueImpl::OpenRangeValueImpl(
		IExpr				*lhs,
		IExpr				*rhs,
		bool				domain_bound) :
		m_lhs(lhs), m_rhs(rhs), m_domain_bound(domain_bound) {

}

OpenRangeValueImpl::~OpenRangeValueImpl() {
	// TODO Auto-generated destructor stub
}

