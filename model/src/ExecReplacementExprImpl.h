/*
 * ExecReplacementExprImpl.h
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
 *  Created on: Apr 20, 2018
 *      Author: ballance
 */
#pragma once
#include "IExecReplacementExpr.h"

class ExecReplacementExprImpl: public IExecReplacementExpr {
public:
	ExecReplacementExprImpl(
			IExpr			*expr,
			uint32_t		offset,
			uint32_t		length);

	virtual ~ExecReplacementExprImpl();

	virtual IExpr *getExpr() const { return m_expr; }

	virtual uint32_t getOffset() const { return m_offset; }

	virtual uint32_t getLength() const { return m_length; }

private:

	IExpr					*m_expr;
	uint32_t				m_offset;
	uint32_t				m_length;

};

