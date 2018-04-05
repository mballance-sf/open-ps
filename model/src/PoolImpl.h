/*
 * PoolImpl.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_POOLIMPL_H_
#define MODEL_SRC_POOLIMPL_H_

#include "FieldImpl.h"
#include "IPool.h"

using namespace psi;

class PoolImpl: public IPool, public psi::FieldImpl {
public:
	PoolImpl(
			const std::string		&name,
			IBaseItem				*pool_type,
			IExpr					*pool_size);

	virtual ~PoolImpl();

	virtual IExpr *getSize() const { return m_pool_size; }

private:

	IExpr							*m_pool_size;


};

#endif /* MODEL_SRC_POOLIMPL_H_ */
