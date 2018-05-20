/*
 * RulesItem.h
 *
 *  Created on: Aug 16, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESITEM_H_
#define SRC_RULESITEM_H_
#include "IBaseItem.h"
#include "BaseItemImpl.h"




namespace qpssc {

class RulesItem {
public:
	enum RulesItemType {
		RulesItem_Action,
		RulesItem_MetaActionImport,
		RulesItem_ActionExecItem,
		RulesItem_TempVar,
		RulesItem_ParallelItem
	};

public:
	RulesItem(RulesItemType type);

	virtual ~RulesItem();

	RulesItemType getItemType() { return m_type; }

private:
	RulesItemType				m_type;

};

} /* namespace qpssc */

#endif /* SRC_RULESITEM_H_ */
