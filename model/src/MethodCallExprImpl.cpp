/*
 * MethodCallExprImpl.cpp
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
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
 * 
 *  Created on: Aug 30, 2016
 *      Author: ballance
 */

#include "MethodCallExprImpl.h"

namespace psi {

MethodCallExprImpl::MethodCallExprImpl(
		IImportFunc					*func,
		const std::vector<IExpr *>	&parameters) :
				ExprImpl(IExpr::ExprType_MethodCall),
				m_func(func), m_parameters(parameters) {

}

MethodCallExprImpl::~MethodCallExprImpl() {
	// TODO Auto-generated destructor stub
}

IExpr *MethodCallExprImpl::clone() const {
	std::vector<IExpr *> parameters;

	for (std::vector<IExpr *>::const_iterator it=getParameters().begin();
			it!=getParameters().end(); it++) {
		parameters.push_back((*it)->clone());
	}

	return new MethodCallExprImpl(getFunc(), parameters);
}

} /* namespace psi */
