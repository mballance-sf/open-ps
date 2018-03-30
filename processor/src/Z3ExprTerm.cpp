/*
 * Z3ExprTerm.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: ballance
 */

#include "Z3ExprTerm.h"

Z3ExprTerm::Z3ExprTerm() : m_expr(0), m_size(0), m_is_signed(false) {

}

Z3ExprTerm::Z3ExprTerm(
		Z3_ast		expr,
		uint32_t	size,
		bool		is_signed) :
				m_expr(expr), m_size(size), m_is_signed(is_signed) {

}

Z3ExprTerm::~Z3ExprTerm() {
	// TODO Auto-generated destructor stub
}

