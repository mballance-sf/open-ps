/*
 * VariableRefImpl.cpp
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
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#include "VariableRefImpl.h"

VariableRefImpl::VariableRefImpl(
		IBaseItem				*scope,
		const std::string		&id,
		IExpr					*index_lhs,
		IExpr					*index_rhs) :
		ExprImpl(IExpr::ExprType_VariableRef),
		m_scope(scope), m_target(0), m_id(id),
		m_index_lhs(index_lhs), m_index_rhs(index_rhs),
		m_next(0) {

}

VariableRefImpl::~VariableRefImpl() {
	// TODO Auto-generated destructor stub
}

std::string VariableRefImpl::toString() const {
	std::string ret = m_id;

	if (m_next) {
		ret += ".";
		ret += m_next->toString();
	}

	return ret;
}

IExpr *VariableRefImpl::clone() const {
	VariableRefImpl *ret = new VariableRefImpl(m_scope, m_id, m_index_lhs, m_index_rhs);
	ret->setNext(getNext());

	return ret;
}
