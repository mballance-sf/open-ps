/*
 * ExecReplacementExprImpl.cpp
 *
 *  Created on: Apr 20, 2018
 *      Author: ballance
 */

#include "ExecReplacementExprImpl.h"

ExecReplacementExprImpl::ExecReplacementExprImpl(
		IExpr		*expr,
		uint32_t	offset,
		uint32_t	length) :
		m_expr(expr), m_offset(offset), m_length(length) {
	// TODO Auto-generated constructor stub

}

ExecReplacementExprImpl::~ExecReplacementExprImpl() {
	// TODO Auto-generated destructor stub
}

