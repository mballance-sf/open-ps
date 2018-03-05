/*
 * QPSSCUtils.h
 *
 *  Created on: Aug 25, 2016
 *      Author: ballance
 */

#ifndef SRC_QPSSCUTILS_H_
#define SRC_QPSSCUTILS_H_
#include <vector>
#include <stdint.h>
#include <string>
#include "IModel.h"

using namespace psi_api;

namespace qpssc {

class QPSSCUtils {
public:
	QPSSCUtils();

	virtual ~QPSSCUtils();

	static int execvp(
			const std::vector<std::string>		&cmdline,
			const std::string					&wd);

	static bool get_roots(
			IModel			*model,
			IComponent		**component,
			IAction			**action);
};

} /* namespace qpssc */

#endif /* SRC_QPSSCUTILS_H_ */
