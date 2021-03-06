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
#include "PSS2ModelVisitor.h"
#include "ResolveRefsProcessor.h"
#include "PSSLexer.h"
#include "PSSParser.h"
#include "ModelVisitor.h"
#include "TestExecListener.h"
#include "EntryFinder.h"
#include "ModelImpl.h"
#include "Z3ModelBuilder.h"
#include "Z3ModelEvaluator.h"
#include "OPSC.h"
#include <ctype.h>

static std::string strip_ws(const std::string &str);

static void run_test(
		const std::string		&content,
		const std::string		&root_component,
		const std::string		&root_action,
		const std::string		&expected);



static std::string strip_ws(const std::string &str) {
	std::string ret;
	uint32_t i=0;

	while (i<str.size()) {
		while (i < str.size() && isspace(str.at(i))) {
			i++;
		}

		// Now, find the end of the line
		while (i < str.size() && str.at(i) != '\n') {
			ret += str.at(i);
			i++;
		}

		ret += "\n";
	}

	return ret;
}


TEST(z3_model,hello_world) {
	const char *src = R"(
		component top {
			action entry {
				rand bit[4]		v, v1, v2, v3, v4;

				constraint c {
					v1 < v2;
					v2 < v3;
					if (v == 1) {
						v1 == 0;
					}
					unique {v, v1, v2, v3, v4};

					v1 in [1, 3..5, 10..13];
				}
				exec body C = """
					printf("Hello World {{v}} {{v1}} {{v2}} {{v3}} {{v4}}\n");
				""";
			}
		}
	)";

	const char *expected = R"(
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
	)";

	run_test(src, "top", "entry", expected);
}

TEST(z3_model,select) {
	const char *src = R"(
		component top {
			action print1 {
				exec body C = """
					printf("Hello World (1)\n");
				""";
			}
			action print2 {
				exec body C = """
					printf("Hello World (2)\n");
				""";
			}
			action print3 {
				exec body C = """
					printf("Hello World (3)\n");
				""";
			}
			action entry {
				rand bit[8] count;
				print1		v1;
				print2		v2;
				print3		v3;

				constraint count <= 10 && count > 0;

				activity {
					repeat (count) {
					select {
						v1;
						v2;
						v3;
					}
					}
				}
			}
		}
	)";

	const char *expected = R"(
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
	)";

	run_test(src, "top", "entry", expected);
}

TEST(z3_model,two_level_action) {
	const char *src = R"(
		component top {
			action sub {
				rand bit[4]		v, v1, v2, v3, v4;
				exec body C = """
					printf("Hello World {{v}} {{v1}} {{v2}} {{v3}} {{v4}}\n");
				""";
			}
			action entry {
				sub s1, s2;
				activity {
					s1;
					s2;
				}
			}
		}
	)";

	const char *expected = R"(
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
	)";

	run_test(src, "top", "entry", expected);
}

TEST(z3_model,repeat_count) {
	const char *src = R"(
		component top {
			action sub {
				rand bit[4]		v, v1, v2, v3, v4;
				exec body C = """
					printf("Hello World {{v}} {{v1}} {{v2}} {{v3}} {{v4}}\n");
				""";
			}
			action entry {
				sub s1, s2;
				activity {
					repeat (5) {
						s1;
					}
				}
			}
		}
	)";

	const char *expected = R"(
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
	)";

	run_test(src, "top", "entry", expected);
}


TEST(z3_model,repeat_fix_one) {
	const char *src = R"(
		component top {
			action sub {
				rand bit[4]		v, v1, v2, v3, v4;
				exec body C = """
					printf("Hello World {{v}} {{v1}} {{v2}} {{v3}} {{v4}}\n");
				""";
			}
			action entry {
				sub s1, s2;
				rand bit[4]	val;
				constraint { s1.v == val; }
				activity {
					repeat (5) {
						s1;
					}
				}
			}
		}
	)";

	const char *expected = R"(
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
begin_exec:
printf("Hello World 0 0 1 0 0\n");
end_exec:
	)";

	run_test(src, "top", "entry", expected);
}

TEST(z3_model,subsys_reg_block) {
//	run_test_file("")
}

static void run_test(
		const std::string		&content,
		const std::string		&root_component,
		const std::string		&root_action,
		const std::string		&expected) {
	OPSC opsc;

	std::istringstream in(content);

	ASSERT_TRUE(opsc.parse(in, "hello_world.pss")); // TODO:

	ASSERT_TRUE(opsc.link());

	ASSERT_TRUE(opsc.elab(root_component, root_action));

	Z3ModelBuilder model_builder;
	Z3ModelH z3_model = model_builder.build(
			opsc.model().get(),
			opsc.root_comp(),
			opsc.root_action());

	Z3ModelEvaluator model_evaluator(z3_model);

	TestExecListener exec_listener(z3_model.get());

	fprintf(stdout, "--> eval\n");
	fflush(stdout);
	model_evaluator.eval(&exec_listener);
	fprintf(stdout, "<-- eval\n");
	fflush(stdout);

	// TODO:
//	z3_processor.build(std::get<0>(entry), std::get<1>(entry));
//
//	z3_processor.set_exec_listener(&exec_listener);
//
//	z3_processor.run();
//
	std::string exp = strip_ws(expected);
	std::string actual = strip_ws(exec_listener.get_result());

	fprintf(stdout, "Expected:\n%s\n", exp.c_str());
	fprintf(stdout, "Actual:\n%s\n", actual.c_str());

	ASSERT_EQ(exp, actual);
}

