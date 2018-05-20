/*
 * ModelNameProvider.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include <string>
#include <vector>
#include "IField.h"



class ModelNameProvider {
public:
	ModelNameProvider();

	virtual ~ModelNameProvider();

	std::string &prefix();

private:

};

