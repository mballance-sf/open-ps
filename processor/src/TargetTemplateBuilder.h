/*
 * TargetTemplateBuilder.h
 *
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */
#pragma once
#include <vector>
#include <string>
#include "IExecReplacementExpr.h"
#include "IItemFactory.h"

class TargetTemplateBuilder {
public:
	TargetTemplateBuilder();

	virtual ~TargetTemplateBuilder();

	static std::vector<IExecReplacementExpr *> build(
			const std::string		&target_template,
			IItemFactory			*factory,
			IBaseItem				*scope);

};

