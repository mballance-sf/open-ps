/*
 * z3_model_tests.cpp
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>
#include <stdint.h>
#include "PSS2PSIVisitor.h"
#include "ResolveRefsProcessor.h"
#include "PSSLexer.h"
#include "PSSParser.h"
#include "PSIVisitor.h"
#include "Z3ModelProcessor.h"
#include "EntryFinder.h"
#include "ModelImpl.h"

using namespace antlr4;
using namespace psi_api;
using namespace psi::apps;

TEST(z3_model,smoke) {
	const char *src = R"(
		component top {
			action entry {
				rand bit[4]		a, b, c;
			}
		}
	)";

	IModel *model = new psi::ModelImpl();
	ResolveRefsProcessor refs_processor;
	Z3ModelProcessor z3_processor;

	std::istringstream in(src);
	ANTLRInputStream input(in);
	PSSLexer lexer(&input);

	CommonTokenStream tokens(&lexer);
	PSSParser parser(&tokens);
	PSSParser::ModelContext *ctxt = parser.model();

	ASSERT_EQ(0, parser.getNumberOfSyntaxErrors());

	PSS2PSIVisitor pss2psi(model, "smoke");
	pss2psi.visitModel(ctxt);

	ASSERT_TRUE(refs_processor.process(model));

	std::tuple<IComponent *, IAction *> entry;
	ASSERT_TRUE(EntryFinder::find(
			model, "top", "entry", entry));

	z3_processor.process(std::get<0>(entry), std::get<1>(entry));
}


