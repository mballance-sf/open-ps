/*
 * ArrayTypeImpl.cpp
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

#include "ArrayTypeImpl.h"
#include "FieldImpl.h"
#include "ScalarTypeImpl.h"
#include "LiteralImpl.h"



ArrayTypeImpl::ArrayTypeImpl(
		IBaseItem				*target,
		bool					has_sum,
		IExpr					*lhs,
		IExpr					*rhs) : BaseItemImpl(IBaseItem::TypeArray) {
	m_target = target;
	if (has_sum) {
		m_sum = new FieldImpl("sum",
			new ScalarTypeImpl(IScalarType::ScalarType_Int,
					new LiteralImpl((uint64_t)63),
					new LiteralImpl((uint64_t)0), 0),
			IField::FieldAttr_None,
			0
		);
		add(m_sum);
	}
	m_size = new FieldImpl("size",
			new ScalarTypeImpl(IScalarType::ScalarType_Bit,
					new LiteralImpl((uint64_t)31),
					new LiteralImpl((uint64_t)0), 0),
			IField::FieldAttr_None,
			0
			);
	add(m_size);
	m_lhs = lhs;
	m_rhs = rhs;
}

ArrayTypeImpl::~ArrayTypeImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ArrayTypeImpl::clone() const {
	return new ArrayTypeImpl(
			m_target,
			hasSum(),
			m_lhs,
			m_rhs);
}
