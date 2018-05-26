/*
 * TargetTemplateBuilder.cpp
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

#include "TargetTemplateBuilder.h"
#include <sstream>
#include "ExprLexer.h"
#include "ExprParser.h"
#include "Expr2ModelVisitor.h"

using namespace antlrcpp;
using namespace antlr4;

TargetTemplateBuilder::TargetTemplateBuilder() {
	// TODO Auto-generated constructor stub

}

TargetTemplateBuilder::~TargetTemplateBuilder() {
	// TODO Auto-generated destructor stub
}

std::vector<IExecReplacementExpr *> TargetTemplateBuilder::build(
		const std::string		&templ,
		IItemFactory			*factory,
		IBaseItem				*scope) {
	std::vector<IExecReplacementExpr *> replacements;

	// Identify all the positions in the target-template
	// string that are template substituions
	// Save these in an offsets list to be used later
	// in compusing the replace-expression list
	uint32_t i=0;
	while (i<templ.size()) {
		if (i+1 < templ.size() &&
				templ.at(i) == '{' &&
				templ.at(i+1) == '{') {
			// Scan forward to the closing }}
			uint32_t start = i;
			i++;
			while (i<templ.size()) {
				if (templ.at(i) == '}' && templ.at(i-1) == '}') {
					break;
				} else {
					i++;
				}
			}

			// Valid template reference
			if (templ.at(i) == '}' && templ.at(i-1) == '}') {
				uint32_t end = i;
				uint32_t len = (end-start+1);
				std::string expr = templ.substr(start+2,
						end-start+1-4);
				fprintf(stdout, "expr=%s offset=%d len=%d\n",
						expr.c_str(), start, len);
				std::stringstream in(expr);
				ANTLRInputStream input(in);
				ExprLexer lexer(&input);
				CommonTokenStream tokens(&lexer);
				ExprParser parser(&tokens);

				ExprParser::EntryContext *ctxt = parser.entry();

				Expr2ModelVisitor visitor(factory, scope);

				fprintf(stdout, "--> process expression\n");
				IExpr *target_expr = visitor.visit(ctxt);
				fprintf(stdout, "<-- process expression\n");

				replacements.push_back(factory->mkExecReplacementExpr(
						target_expr, start, (end-start+1)));

			} else {
				fprintf(stdout, "bad template ref\n");
			}
		}
		i++;
	}

	return replacements;
}

