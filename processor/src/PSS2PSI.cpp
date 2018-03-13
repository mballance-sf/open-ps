/*
 * PSS2PSI.cpp
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

