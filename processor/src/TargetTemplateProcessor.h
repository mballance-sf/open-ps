/*
 * TargetTemplateProcessor.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
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

