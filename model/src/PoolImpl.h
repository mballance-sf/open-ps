/*
 * PoolImpl.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 *
 *  Created on: Apr 4, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_POOLIMPL_H_
#define MODEL_SRC_POOLIMPL_H_

#include "FieldImpl.h"
#include "IPool.h"



class PoolImpl: public IPool, public FieldImpl {
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
