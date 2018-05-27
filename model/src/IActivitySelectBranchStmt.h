/*
 * IActivitySelectBranchStmt.h
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#pragma once
#include "IActivityStmt.h"
#include "IExpr.h"


class IActivitySelectBranchStmt {
public:

	virtual ~IActivitySelectBranchStmt() { }

	virtual IExpr *getGuard() const = 0;

	virtual IExpr *getWeight() const = 0;

	virtual IActivityStmt *getStmt() const = 0;

};



