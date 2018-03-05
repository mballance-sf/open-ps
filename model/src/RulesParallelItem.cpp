/*
 * RulesParallelItem.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: ballance
 */

#include "RulesParallelItem.h"

namespace qpssc {

RulesParallelItem::RulesParallelItem(const std::string &name, RulesParallelItem::ParallelItemType t) :
	RulesItem(RulesItem::RulesItem_ParallelItem), NamedItemImpl(name) {

	m_item_type = t;
}

RulesParallelItem::~RulesParallelItem() {
	// TODO Auto-generated destructor stub
}

} /* namespace qpssc */
