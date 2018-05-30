/*
 * parser_tests.cpp
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
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"
#include <dirent.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "PSS2ModelVisitor.h"
#include "ResolveRefsProcessor.h"
#include "PSSLexer.h"
#include "PSSParser.h"
#include "ModelVisitor.h"
#include "ModelImpl.h"
#include "Model2XML.h"
#include "OPSC.h"

using namespace antlr4;



#ifndef TESTS_DATA_DIR
#define TESTS_DATA_DIR "/unknown"
#endif

#define DATA_DIR(data_dir, extension) data_dir extension

typedef std::pair<std::string, std::string> TestParam;

class pss_processor : public testing::TestWithParam<TestParam> {};

class CheckRefsResolvedVisitor : public ModelVisitor {
public:

	virtual void visit_ref_type(IRefType *ref) override {
		std::string type;
		for (uint32_t i=0; i<ref->getTypeId().size(); i++) {
			type += ref->getTypeId().at(i);

			if (i+1 < ref->getTypeId().size()) {
				type += "::";
			}
		}

		if (!ref->getTargetType()) {
			fprintf(stdout, "Failed to resolve type %s\n",
					type.c_str());
		}
		ASSERT_TRUE((ref->getTargetType() != (IBaseItem *)0));
	}

};

TEST_P(pss_processor,spec_examples) {
	CheckRefsResolvedVisitor v;
	ResolveRefsProcessor	 refs_processor;
	OPSC opsc;
	IModel *model = new ModelImpl();

//	fprintf(stdout, "--> %s\n", GetParam().second.c_str());

	std::ifstream in(GetParam().second);
	ASSERT_EQ(true, in.is_open());


	ASSERT_TRUE(opsc.parse(in, GetParam().second));

	ASSERT_TRUE(opsc.link());

//	if (in.is_open()) {
//
//		ANTLRInputStream input(in);
//		PSSLexer lexer(&input);
//
//		CommonTokenStream tokens(&lexer);
//		PSSParser parser(&tokens);
//		PSSParser::ModelContext *ctxt = parser.model();
//
//		ASSERT_EQ(0, parser.getNumberOfSyntaxErrors());
//
//		PSS2ModelVisitor pss2psi(model, GetParam().second);
//		pss2psi.visitModel(ctxt);
//
////		GTEST_TEST_BOOLEAN_()
//		ASSERT_TRUE(refs_processor.process(model));
//
//		v.visit_model(model);
//
//		Model2XML m2x;
//		std::string xml = m2x.convert(model);
//		fprintf(stdout, "XML:\n%s\n", xml.c_str());
//	}

//	fprintf(stdout, "<-- %s\n", GetParam().second.c_str());

	delete model;
}

static std::vector<TestParam> ReadTestCasesFromDisk() {
	std::vector<TestParam> ret;
	struct dirent *ent;
	DIR *spec_examples;
//	const char *spec_examples_dir = ::getenv("SPEC_EXAMPLES");
	const char *spec_examples_dir = DATA_DIR(TESTS_DATA_DIR, "/spec_examples");

	EXPECT_TRUE((spec_examples_dir != 0));

	if (spec_examples_dir) {

		spec_examples = opendir(spec_examples_dir);
		std::string spec_examples_dir_s = spec_examples_dir;

		while ((ent = readdir(spec_examples))) {
			if (strstr(ent->d_name, ".pss") &&
					ent->d_name[0] != '.') {
				std::string name = ent->d_name;
				name = name.substr(0, name.size()-4);
				std::string path = spec_examples_dir_s + "/" + ent->d_name;
				ret.push_back(std::pair<std::string,std::string>(name, path));
			}
		}
	} else {
		fprintf(stdout, "Error: SPEC_EXAMPLES not set\n");
		fflush(stdout);
	}

	return ret;
}

static std::string GetTestCaseName(const ::testing::TestParamInfo<TestParam> &info) {
	return info.param.first;
}

INSTANTIATE_TEST_CASE_P(
    SpecExamples,
    pss_processor,
    testing::ValuesIn(ReadTestCasesFromDisk()),
	GetTestCaseName);


