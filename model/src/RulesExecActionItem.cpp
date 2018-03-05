/*
 * RulesExecActionItem.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "RulesExecActionItem.h"

namespace qpssc {

RulesExecActionItem::RulesExecActionItem(const std::string &name) :
	FieldImpl(name, 0, IField::FieldAttr_None, 0),
	RulesItem(RulesItem_ActionExecItem) {

	setType(IBaseItem::TypeVendor);

	m_exec.setParent(this);
}

RulesExecActionItem::~RulesExecActionItem() {
	// TODO Auto-generated destructor stub
}

ExecImpl *RulesExecActionItem::getExec() {
	return &m_exec;
}


} /* namespace qpssc */
