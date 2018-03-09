/*
 * RulesTempVar.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESTEMPVAR_H_
#define SRC_RULESTEMPVAR_H_

#include <FieldImpl.h>

#include "RulesItem.h"

namespace qpssc {

// Represents a field that is outside the graph
class RulesTempVar: public psi::FieldImpl, public virtual RulesItem {
public:
	RulesTempVar(const std::string &name);

	virtual ~RulesTempVar();
};

} /* namespace qpssc */

#endif /* SRC_RULESTEMPVAR_H_ */
