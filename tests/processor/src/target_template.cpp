/*
 * parser_tests.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "ItemFactoryImpl.h"
#include "TargetTemplateBuilder.h"
#include "TargetTemplateProcessor.h"
#include "TestVarValueProvider.h"

using namespace psi_api;

static void run_test(
		const std::map<std::string, VarVal>	&var_map,
		const std::string					&context,
		const std::string 					&content,
		const std::string 					&exp) {
	ItemFactoryImpl factory;
	TestVarValueProvider var_provider(var_map);

	std::vector<IExecReplacementExpr *> replacements =
			TargetTemplateBuilder::build(content, &factory, 0);

	TargetTemplateProcessor processor(&var_provider);
	std::string result = processor.process(context, content, replacements);

	ASSERT_EQ(result, exp);
}

TEST(target_template,literals) {
	std::map<std::string, VarVal> var_map;

	const char *content = R"(
	a b {{"c"}} {{1}} {{2}} 3 4
	)";

	const char *exp = R"(
	a b "c" 1 2 3 4
	)";

	run_test(var_map, "", content, exp);
}

TEST(target_template,single_var) {
	std::map<std::string, VarVal> var_map;

	var_map["v1"] = VarVal("c");
	var_map["v2"] = VarVal((uint64_t)1);
	var_map["v3"] = VarVal((uint64_t)2);

	fprintf(stdout, "v1.type=%d\n", var_map.find("v1")->second.type);

	const char *content = R"(
	a b {{v1}} {{v2}} {{v3}} 3 4
	)";

	const char *exp = R"(
	a b "c" 1 2 3 4
	)";

	run_test(var_map, "", content, exp);
}

TEST(target_template,plus_same_sign) {
	std::map<std::string, VarVal> var_map;

	var_map["v1"] = VarVal("c");
	var_map["v2"] = VarVal((uint64_t)1);
	var_map["v3"] = VarVal((uint64_t)2);

	fprintf(stdout, "v1.type=%d\n", var_map.find("v1")->second.type);

	const char *content = R"(
	a b {{1+2}} 3 4
	)";

	const char *exp = R"(
	a b 3 3 4
	)";

	run_test(var_map, "", content, exp);
}
