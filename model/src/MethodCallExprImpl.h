/*
 * MethodCallExprImpl.h
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

#ifndef INCLUDED_METHOD_CALL_EXPR_IMPL_H
#define INCLUDED_METHOD_CALL_EXPR_IMPL_H

#include "ExprImpl.h"
#include "BaseItemImpl.h"
#include "IMethodCallExpr.h"
#include <vector>

namespace psi {

class MethodCallExprImpl:
		public virtual ExprImpl,
		public virtual IMethodCallExpr {
public:

	MethodCallExprImpl(
			IImportFunc					*func,
			const std::vector<IExpr *>	&parameters);

	MethodCallExprImpl(
			IImportFunc					*func,
			std::initializer_list<IExpr *>	&params) :
				MethodCallExprImpl(func, std::vector<IExpr *>(params)) { }

	virtual ~MethodCallExprImpl();

	virtual IImportFunc *getFunc() const { return m_func; }

	virtual const std::vector<IExpr *> &getParameters() const { return m_parameters; }

	virtual IExpr *clone() const;

private:
	IImportFunc						*m_func;
	std::vector<IExpr *>			m_parameters;
};

} /* namespace psi */

#endif /* INCLUDED_METHOD_CALL_EXPR_IMPL_H */
