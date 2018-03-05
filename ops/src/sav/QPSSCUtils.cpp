/*
 * QPSSCUtils.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: ballance
 */

#include "QPSSCUtils.h"
#ifdef _WIN32
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace qpssc {

QPSSCUtils::QPSSCUtils() {
	// TODO Auto-generated constructor stub

}

QPSSCUtils::~QPSSCUtils() {
	// TODO Auto-generated destructor stub
}

#ifdef _WIN32
int QPSSCUtils::execvp(
		const std::vector<std::string>		&cmdline,
		const std::string					&wd) {
	return -1;
}
#else
int QPSSCUtils::execvp(
		const std::vector<std::string>		&cmdline,
		const std::string					&wd) {
	pid_t pid;
	int ret;

	if ((pid = fork()) == 0) {
		char *argv[256];

		for (uint32_t i=0; i<cmdline.size(); i++) {
			argv[i] = strdup(cmdline.at(i).c_str());
		}
		argv[cmdline.size()] = 0;

		// Child
		::chdir(wd.c_str());

		::execvp(argv[0], argv);

		fprintf(stdout, "Error: exec failed\n");

		exit(1);
	}

	// Wait for the child to terminate
	::waitpid(pid, &ret, 0);

	return ret;
}
#endif

bool QPSSCUtils::get_roots(
		IModel				*model,
		IComponent			**component,
		IAction				**action) {
	IComponent *c = 0;
	IAction *a = 0;

	*component = 0;
	*action = 0;

	// Find the root component and action
	for (std::vector<IBaseItem *>::const_iterator it=model->getItems().begin();
			it!=model->getItems().end(); it++) {
		IBaseItem *item = *it;
		if ((c=dynamic_cast<IComponent *>(item)) &&
				c->hasAttribute("ROOT")) {
			break;
		} else {
			c = 0;
		}
	}

	if (!c) {
		fprintf(stdout, "Error: Failed to find root component\n");
		return false;
	}

	for (std::vector<IBaseItem *>::const_iterator it=c->getItems().begin();
			it!=c->getItems().end(); it++) {
		IBaseItem *item = *it;
		if ((a=dynamic_cast<IAction *>(item)) &&
				a->hasAttribute("ROOT")) {
			break;
		} else {
			a = 0;
		}
	}

	if (!a) {
		fprintf(stdout, "Error: failed to find root action\n");
		return false;
	}

	*component = c;
	*action = a;

	return true;
}

} /* namespace qpssc */
