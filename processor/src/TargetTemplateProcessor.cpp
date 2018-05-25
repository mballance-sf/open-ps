/*
 * TargetTemplateProcessor.cpp
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

#include "TargetTemplateProcessor.h"

TargetTemplateProcessor::TargetTemplateProcessor(IVarValueProvider *p) :
	m_var_provider(p), m_expr_eval(p) {

}

TargetTemplateProcessor::~TargetTemplateProcessor() {
	// TODO Auto-generated destructor stub
}

std::string TargetTemplateProcessor::process(
		const std::string							&context,
		const std::string							&templ,
		const std::vector<IExecReplacementExpr *> 	&replacements) {
	char tmp[128];
	std::string ret;
	uint32_t start=0;

	for (uint32_t i=0; i<replacements.size(); i++) {
		IExecReplacementExpr *r = replacements.at(i);
		if (start < r->getOffset()) {
			ret.append(templ.substr(start, (r->getOffset()-start)));
		}

		fprintf(stdout, "expr=%p\n", r->getExpr());
		VarVal v = m_expr_eval.eval(context, r->getExpr());

		switch (v.type) {
		case VarVal_Uint: {
			sprintf(tmp, "%llx", v.ui);
			ret.append(tmp);
		} break;
		case VarVal_Int: {
			sprintf(tmp, "%lld", v.si);
			ret.append(tmp);
		} break;
		case VarVal_Bool: {
			ret.append((v.b)?"true":"false");
		} break;
		case VarVal_String: {
			fprintf(stdout, "ret.s=%s\n", v.s.c_str());
			ret.append("\"");
			ret.append(v.s);
			ret.append("\"");
		} break;
		}

		start = r->getOffset() + r->getLength();
	}

	if (start < templ.size()) {
		ret.append(templ.substr(start, (templ.size()-start)));
	}

	return ret;
}
