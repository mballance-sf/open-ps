/*
 * PoolImpl.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: ballance
 */

#include "PoolImpl.h"

PoolImpl::PoolImpl(
		const std::string		&name,
		IBaseItem				*pool_type,
		IExpr					*pool_size) :
		FieldImpl(name, pool_type, IField::FieldAttr_Pool, 0),
		m_pool_size(pool_size) {
}

PoolImpl::~PoolImpl() {
	// TODO Auto-generated destructor stub
}

