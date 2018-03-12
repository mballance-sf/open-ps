/*
 * parser_tests.cpp
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
#include "PSSLexer.h"
#include "PSSParser.h"

using namespace antlr4;

#ifndef TESTS_DATA_DIR
#define TESTS_DATA_DIR "/unknown"
#endif

#define DATA_DIR(data_dir, extension) data_dir extension

typedef std::pair<std::string, std::string> TestParam;

class pss_parser : public testing::TestWithParam<TestParam> {};

TEST_P(pss_parser,spec_examples) {
	std::ifstream in(GetParam().second);
	if (in.is_open()) {
		ANTLRInputStream input(in);
		PSSLexer lexer(&input);

		CommonTokenStream tokens(&lexer);
		PSSParser parser(&tokens);
		PSSParser::ModelContext *ctxt = parser.model();

		ASSERT_EQ(0, parser.getNumberOfSyntaxErrors());
	}
}

static std::vector<TestParam> ReadTestCasesFromDisk() {
	std::vector<TestParam> ret;
	struct dirent *ent;
	DIR *spec_examples;
	std::string spec_examples_dir = DATA_DIR(TESTS_DATA_DIR, "/spec_examples");

	spec_examples = opendir(spec_examples_dir.c_str());

	while ((ent = readdir(spec_examples))) {
		if (strstr(ent->d_name, ".pss")) {
			std::string name = ent->d_name;
			name = name.substr(0, name.size()-4);
			std::string path = spec_examples_dir + "/" + ent->d_name;
			ret.push_back(std::pair<std::string,std::string>(name, path));
		}
	}

	return ret;
}

static std::string GetTestCaseName(const ::testing::TestParamInfo<TestParam> &info) {
	return info.param.first;
}

INSTANTIATE_TEST_CASE_P(
    SpecExamples,
    pss_parser,
    testing::ValuesIn(ReadTestCasesFromDisk()),
	GetTestCaseName);


