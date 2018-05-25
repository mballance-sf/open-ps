/*
 * ops_main.cpp
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
#include <stdarg.h>
#include <stdio.h>

static void fatal(const char *fmt, ...);

/**
 * ops [options] [sources]
 */
int main(int argc, char **argv) {
	OPS *ops = new OPS();

	for (int i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			fatal("unknown option %s\n", argv[i]);
		} else {
			ops->add_source_file(argv[i]);
		}
	}

	if (!ops->parse()) {
		exit(1);
	}
}

static void fatal(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");

	va_end(ap);
	exit(1);
}
