/*
 * RulesActionItem.h
 *
 *  Created on: Aug 17, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESACTIONITEM_H_
#define SRC_RULESACTIONITEM_H_

#include "RulesItem.h"
#include "NamedItemImpl.h"

using namespace psi;

namespace qpssc {

class RulesActionItem:
		public virtual RulesItem,
		public virtual NamedItemImpl {
public:
	RulesActionItem(const std::string &name);

	virtual ~RulesActionItem();
};

} /* namespace qpssc */

#endif /* SRC_RULESACTIONITEM_H_ */
