/*
 * RulesParallelItem.h
 *
 *  Created on: Jan 25, 2017
 *      Author: ballance
 */

#ifndef SRC_RULESPARALLELITEM_H_
#define SRC_RULESPARALLELITEM_H_

#include "RulesItem.h"
#include "NamedItemImpl.h"

namespace qpssc {

class RulesParallelItem:
		public virtual RulesItem,
		public virtual NamedItemImpl {
public:
	enum ParallelItemType {
		ParallelContextCreate,
		ParallelThreadBegin,
		ParallelThreadEnd,
		ParallelContextJoin
	};

	RulesParallelItem(const std::string &name, RulesParallelItem::ParallelItemType t);

	virtual ~RulesParallelItem();

	ParallelItemType getItemType() const { return m_item_type; }


private:
	ParallelItemType				m_item_type;
};

} /* namespace qpssc */

#endif /* SRC_RULESPARALLELITEM_H_ */
