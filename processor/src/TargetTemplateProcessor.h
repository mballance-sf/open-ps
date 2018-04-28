/*
 * TargetTemplateProcessor.h
 *
 *  Created on: Apr 28, 2018
 *      Author: ballance
 */
#pragma once
#include <vector>
#include <string>
#include "IExecReplacementExpr.h"
#include "IVarValueProvider.h"
#include "ExprEvaluator.h"

class TargetTemplateProcessor {
public:
	TargetTemplateProcessor(IVarValueProvider *var_provider);

	virtual ~TargetTemplateProcessor();

	std::string process(
			const std::string							&context,
			const std::string							&templ,
			const std::vector<IExecReplacementExpr *>	&replacements
			);

private:
	ExprEvaluator					m_expr_eval;
	IVarValueProvider				*m_var_provider;

};

