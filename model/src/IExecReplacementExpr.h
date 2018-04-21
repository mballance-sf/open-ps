/*
 * IExecReplacementExpr.h
 *
 *  Created on: Apr 20, 2018
 *      Author: ballance
 */
#include "IExpr.h"
#include <stdint.h>
#pragma once

using namespace psi_api;

class IExecReplacementExpr {
public:
	virtual ~IExecReplacementExpr() { }

	virtual IExpr *getExpr() const = 0;

	virtual uint32_t getOffset() const = 0;

	virtual uint32_t getLength() const = 0;

};
