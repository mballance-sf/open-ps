/*
 * RulesPSI2XML.h
 *
 *  Created on: Aug 22, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESPSI2XML_H_
#define SRC_RULESPSI2XML_H_

#include "PSI2XML.h"

namespace qpssc {

class RulesPSI2XML: public psi::apps::PSI2XML {
public:
	RulesPSI2XML();
	virtual ~RulesPSI2XML();
};

} /* namespace qpssc */

#endif /* SRC_RULESPSI2XML_H_ */
