/*
 * InExprImpl.h
 *
 *  Created on: Apr 8, 2018
 *      Author: ballance
 */

#pragma once

#include "ExprImpl.h"
#include "IInExpr.h"

class InExprImpl: public virtual ExprImpl, virtual public IInExpr {
public:
	InExprImpl(IExpr *lhs, IOpenRangeList *rhs);

	virtual ~InExprImpl();

	virtual IExpr *getLhs() const { return m_lhs; }

	virtual IOpenRangeList *getRhs() const { return m_rhs; }

	virtual IExpr *clone() const;

private:
	IExpr					*m_lhs;
	IOpenRangeList			*m_rhs;
};

