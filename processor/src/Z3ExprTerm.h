/*
 * Z3ExprTerm.h
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

#ifndef PROCESSOR_SRC_Z3EXPRTERM_H_
#define PROCESSOR_SRC_Z3EXPRTERM_H_
#include "z3.h"
#include <stdint.h>

class Z3ExprTerm {
public:
	Z3ExprTerm();

	Z3ExprTerm(
			Z3_ast		expr,
			uint32_t	size,
			bool		is_signed
			);

	virtual ~Z3ExprTerm();

	Z3_ast expr() const { return m_expr; }

	uint32_t size() const { return m_size; }

	bool is_signed() const { return m_is_signed; }

private:
	Z3_ast				m_expr;
	uint32_t			m_size;
	bool				m_is_signed;
};

#endif /* PROCESSOR_SRC_Z3EXPRTERM_H_ */
