/*
 * IActivitySelectStmt.h
 *
 *  Created on: May 27, 2018
 *      Author: ballance
 */

#pragma once

#include <vector>
#include "IActivityStmt.h"
#include "IActivitySelectBranchStmt.h"


class IActivitySelectStmt : public virtual IActivityStmt {
public:

	virtual ~IActivitySelectStmt() { }

	virtual const std::vector<IActivitySelectBranchStmt *> &getBranches() const = 0;

};



