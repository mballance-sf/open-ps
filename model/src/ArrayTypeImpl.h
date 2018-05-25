/*
 * ArrayTypeImpl.h
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
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ARRAYTYPEIMPL_H_
#define MODEL_SRC_ARRAYTYPEIMPL_H_

#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "IArrayType.h"



class ArrayTypeImpl: public BaseItemImpl,
	public virtual ScopeItemImpl, public virtual IArrayType {
public:
	ArrayTypeImpl(
			IBaseItem		*target,
			bool			has_sum,
			IExpr			*lhs,
			IExpr			*rhs
			);

	virtual ~ArrayTypeImpl();

	virtual bool hasSum() const { return (m_sum); }

	virtual IField *getSize() const { return m_size; }

	virtual IField *getSum() const { return m_sum; }

	virtual IBaseItem *getTargetType() const { return m_target; }

	virtual IExpr *getLhs() const { return m_lhs; }

	virtual IExpr *getRhs() const { return m_rhs; }

	virtual IBaseItem *clone() const;

private:
	IBaseItem				*m_target;
	IField					*m_size;
	IField					*m_sum;
	IExpr					*m_lhs;
	IExpr					*m_rhs;
};

#endif /* MODEL_SRC_ARRAYTYPEIMPL_H_ */
