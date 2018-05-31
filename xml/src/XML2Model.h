/*
 * XML2Model.h
 *
 *  Created on: May 25, 2018
 *      Author: ballance
 */
#pragma once
#include "IModel.h"
#include <iostream>

class XML2Model {
public:
	XML2Model();

	virtual ~XML2Model();

	bool read(
			std::istream		&in,
			const std::string	&in_name,
			IModel				*model);

};

