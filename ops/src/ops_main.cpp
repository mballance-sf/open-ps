/*
 * ops_main.cpp
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
