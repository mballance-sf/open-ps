/*
 * PSS2PSI.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#pragma once
#include "IModel.h"
#include <string>



class PSS2PSI {
public:
	PSS2PSI();

	virtual ~PSS2PSI();

	bool process(IModel *model, const std::string &path);
};


