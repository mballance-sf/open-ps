/*
 * IVarValueProvider.h
 *
 *  Created on: Apr 24, 2018
 *      Author: ballance
 */
#pragma once
#include <string>
#include "VarVal.h"


class IVarValueProvider {
public:

	virtual ~IVarValueProvider() { }

	virtual VarVal get_value(const std::string &path) = 0;

};



