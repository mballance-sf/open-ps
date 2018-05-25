/*
 * ConstraintForeachImpl.cpp
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
 *  Created on: Mar 28, 2018
 *      Author: ballance
 */

#include "ConstraintForeachImpl.h"

ConstraintForeachImpl::ConstraintForeachImpl(
		IVariableRef			*target,
		const std::string		&iterator,
		IConstraintBlock		*body) : BaseItemImpl(IBaseItem::TypeConstraint) {
	m_target = target;
	m_iterator = iterator;
	m_body = body;
}

ConstraintForeachImpl::~ConstraintForeachImpl() {
	// TODO Auto-generated destructor stub
}

IBaseItem *ConstraintForeachImpl::clone() const {
	return new ConstraintForeachImpl(
			m_target,
			m_iterator,
			m_body);
}
