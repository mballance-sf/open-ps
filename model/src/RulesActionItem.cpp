/*
 * RulesActionItem.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: ballance
 */

#include "RulesActionItem.h"

namespace qpssc {

RulesActionItem::RulesActionItem(const std::string &name) :
	RulesItem(RulesItem::RulesItem_Action),
	NamedItemImpl(name) {

}

RulesActionItem::~RulesActionItem() {
	// TODO Auto-generated destructor stub
}

} /* namespace qpssc */
