/*
 * IInExpr.h
 *
 *  Created on: Apr 7, 2018
 *      Author: ballance
 */
#pragma once
#include "IExpr.h"
#include "IOpenRangeList.h"

using namespace psi_api;

class IInExpr : public virtual IExpr {
public:

	virtual ~IInExpr() { }

	virtual IExpr *getLhs() const = 0;

	virtual IOpenRangeList *getRhs() const = 0;

};


