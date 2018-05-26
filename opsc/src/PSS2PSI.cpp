/*
 * PSS2PSI.cpp
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
 *  Created on: Nov 16, 2017
 *      Author: ballance
 */

#include "PSS2PSI.h"

#include "PSSLexer.h"
#include "PSSParser.h"
#include "PSSBaseVisitor.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "PSS2PSIVisitor.h"

using namespace antlr4;

PSS2PSI::PSS2PSI() {
	// TODO Auto-generated constructor stub

}

PSS2PSI::~PSS2PSI() {
	// TODO Auto-generated destructor stub
}

bool PSS2PSI::process(IModel *model, const std::string &path) {
	std::ifstream in(path);

	if (in.is_open()) {
		PSS2PSIVisitor visitor(model, path);
		ANTLRInputStream input(in);
		PSSLexer lexer(&input);

		CommonTokenStream tokens(&lexer);

		PSSParser parser(&tokens);

		PSSParser::ModelContext *ctxt = parser.model();

		try {
			visitor.visit(ctxt);
		} catch (std::string &m) {
			fprintf(stdout, "Error: %s\n", m.c_str());
			return false;
		}

		return true;
	} else {
		return false;
	}
}

