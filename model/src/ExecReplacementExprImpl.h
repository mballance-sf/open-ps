/*
 * ExecReplacementExprImpl.h
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

