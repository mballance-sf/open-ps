/*
 * Z3ExprTerm.h
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
