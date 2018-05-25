/*
 * OPS.cpp
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
 *  Created on: Mar 5, 2018
 *      Author: ballance
 */

#include "OPS.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "../../processor/src/PSS2PSIVisitor.h"
#include "PSSLexer.h"
#include "PSSParser.h"
#include "ModelImpl.h"

using namespace antlr4;

OPS::OPS() {
	// TODO Auto-generated constructor stub

	m_model = new ModelImpl();
}

OPS::~OPS() {
	// TODO Auto-generated destructor stub
}

void OPS::add_source_file(const std::string &file) {
	m_source_files.push_back(file);
}

bool OPS::parse() {
	bool ret = true;
	if (m_source_files.size() == 0) {
		ret = false;
		fprintf(stdout, "Error: no source files specified\n");
	} else {
	for (std::vector<std::string>::const_iterator it=m_source_files.begin();
			it!=m_source_files.end(); it++) {
		fprintf(stdout, "Note: parsing %s\n", (*it).c_str());
		std::ifstream in(*it);
		if (in.is_open()) {
			ANTLRInputStream input(in);
			PSSLexer lexer(&input);

			CommonTokenStream tokens(&lexer);
			PSSParser parser(&tokens);
			PSSParser::ModelContext *ctxt = parser.model();

			if (parser.getNumberOfSyntaxErrors() > 0) {
				fprintf(stdout, "Error: parse errors while processing %s\n",
						(*it).c_str());
				ret = false;
				break;
			}

			PSS2PSIVisitor pss2psi_visitor(m_model, (*it));
			pss2psi_visitor.visitModel(ctxt);

		} else {
			fprintf(stdout, "Error: failed to open file\n");
			ret = false;
			break;
		}
	}
	}

	return ret;
}

