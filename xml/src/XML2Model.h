/*
 * XML2Model.h
 *
 *  Created on: May 25, 2018
 *      Author: ballance
 */
#pragma once
#include "IModel.h"

class XML2Model {
public:
	XML2Model();

	virtual ~XML2Model();

	IModelPtr xml2model();

};

