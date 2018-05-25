/*
 * z3_model_tests.cpp
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
#include "EntryFinder.h"
#include "ModelImpl.h"

using namespace antlr4;



//TEST(z3_model,smoke) {
//	const char *src = R"(
//		component top {
//			action entry {
//				rand bit[16]	a, b, c, d, e, f;
//				rand bit[4]		s1, s2, s3;
//
//				constraint c {
//					a < 4;
//					a > 0;
//					if (a==1) {
//						b==1;
//					}
//					if (a!=1) {
//						b==2;
//					}
////					(b==1 || b==2 || b==4 || b==8);
//				}
//			}
//		}
//	)";
//
//	IModel *model = new ModelImpl();
//	ResolveRefsProcessor refs_processor;
//	Z3ModelProcessor z3_processor;
//
//	std::istringstream in(src);
//	ANTLRInputStream input(in);
//	PSSLexer lexer(&input);
//
//	CommonTokenStream tokens(&lexer);
//	PSSParser parser(&tokens);
//	PSSParser::ModelContext *ctxt = parser.model();
//
//	ASSERT_EQ(0, parser.getNumberOfSyntaxErrors());
//
//	PSS2PSIVisitor pss2psi(model, "smoke");
//	pss2psi.visitModel(ctxt);
//
//	ASSERT_TRUE(refs_processor.process(model));
//
//	std::tuple<IComponent *, IAction *> entry;
//	ASSERT_TRUE(EntryFinder::find(
//			model, "top", "entry", entry));
//
//	z3_processor.build(std::get<0>(entry), std::get<1>(entry));
//
//	for (uint32_t i=0; i<10; i++) {
//		z3_processor.run();
//	}
//}

//TEST(z3_model,hierarchy) {
//	const char *src = R"(
//		component top {
//			struct S {
//				rand bit[4]		a, b, c;
//			}
//			action entry {
//				rand S		a1, a2, a3;
//			}
//		}
//	)";
//
//	IModel *model = new ModelImpl();
//	ResolveRefsProcessor refs_processor;
//	Z3ModelProcessor z3_processor;
//
//	std::istringstream in(src);
//	ANTLRInputStream input(in);
//	PSSLexer lexer(&input);
//
//	CommonTokenStream tokens(&lexer);
//	PSSParser parser(&tokens);
//	PSSParser::ModelContext *ctxt = parser.model();
//
//	ASSERT_EQ(0, parser.getNumberOfSyntaxErrors());
//
//	PSS2PSIVisitor pss2psi(model, "smoke");
//	pss2psi.visitModel(ctxt);
//
//	ASSERT_TRUE(refs_processor.process(model));
//
//	std::tuple<IComponent *, IAction *> entry;
//	ASSERT_TRUE(EntryFinder::find(
//			model, "top", "entry", entry));
//
//	z3_processor.build(std::get<0>(entry), std::get<1>(entry));
//}

