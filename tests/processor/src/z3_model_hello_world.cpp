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

TEST(z3_model,hello_world) {
	const char *src = R"(
		component top {
			action entry {
				rand bit[4]		v, v1, v2, v3, v4;
				exec body C = """
					printf("Hello World {{v}}\n");
				""";
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

	PSS2PSIVisitor pss2psi(model, "hello_world.pss");
	pss2psi.visitModel(ctxt);

	ASSERT_TRUE(refs_processor.process(model));

	std::tuple<IComponent *, IAction *> entry;
	ASSERT_TRUE(EntryFinder::find(
			model, "top", "entry", entry));

	z3_processor.build(std::get<0>(entry), std::get<1>(entry));

	for (uint32_t i=0; i<10; i++) {
		z3_processor.run();
	}
}

