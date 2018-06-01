/*
 * opsc_main.cpp
 *
 *  Created on: May 28, 2018
 *      Author: ballance
 */
#include "OPSC.h"
#include <vector>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>

static void printhelp() {

}

static void error_exit(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "Error: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);

	printhelp();
	exit(1);
}

/**
 *
 */
int main(int argc, char **argv) {
	std::vector<std::string> files;
	bool verbose = false;
	bool link = false;
	bool elab = false;
	bool incr = false;
	std::string action, component;
	std::string out;

	for (uint32_t i=1; i<argc; i++) {
		std::string arg = argv[i];

		if (arg.at(0) == '-') {
			// Option
			if (arg == "-v" || arg == "--verbose") {
				verbose = true;
			} else if (arg == "-link") {
				link = true;
			} else if (arg == "-c") {
				incr = true;
			} else if (arg == "-action") {
				i++;
				action = argv[i];
			} else if (arg == "-component") {
				i++;
				component = argv[i];
			} else if (arg == "-o") {
				i++;
				out = argv[i];
			} else {
				error_exit("unknown option \"%s\"", arg.c_str());
			}
		} else {
			int32_t dot_idx = arg.find_last_of('.');

			if (dot_idx >= 0) {
				std::string ext = arg.substr(dot_idx);
				if (ext == ".pss" ||
						ext == ".psi") {
					files.push_back(arg);
				} else {
					error_exit("file \"%s\" has an unrecognized extension \"%s\"",
							arg.c_str(), ext.c_str());
				}
			} else {
				error_exit("file \"%s\" doesn't have an extension", arg.c_str());
			}
		}
	}

	if (incr) {
		elab = false;
		if (link) {
			fprintf(stdout, "Error: cannot specify -c and -link together\n");
			exit(1);
		}
	} else if (!link) {
		elab = true;
	}

	if (files.size() == 0) {
		error_exit("no file specified");
	}

	OPSC opsc;
	for (std::vector<std::string>::const_iterator it=files.begin();
			it!=files.end(); it++) {
		const std::string &file = *it;
		const std::string ext = file.substr(file.find_last_of('.'));

		std::ifstream in(file);

		if (in.is_open()) {
			if (ext == ".pss") {
				if (!opsc.parse(in, file)) {
					fprintf(stdout, "Error: failed to parse file \"%s\"\n",
							file.c_str());
					exit(1);
				}
			} else if (ext == ".psi") {
				fprintf(stdout, "Error: .psi files not supported\n");
				exit(1);
			}
			in.close();
		} else {
			fprintf(stdout, "Error: failed to open file \"%s\"\n",
					file.c_str());
			exit(1);
		}
	}

	if (link || elab) {
		if (!opsc.link()) {
			exit(1);
		}
	}

	if (elab) {
		if (!opsc.elab(component, action)) {
			exit(1);
		}
	}

	// Finally, write out the result
	if (out == "") {
		if (files.size() == 1) {
			out = files.at(0).substr(0,
					files.at(0).find_last_of('.'));
			if (elab) {
				out += ".pse";
			} else {
				out += ".psi";
			}
		} else {
			out = "p.out";
		}
	}

	std::ofstream out_f(out);
	if (!opsc.write(out_f)) {
		exit(1);
	}
	out_f.close();

	exit(0);
}



