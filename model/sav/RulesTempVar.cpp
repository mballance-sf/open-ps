/*
 * RulesTempVar.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "RulesTempVar.h"

namespace qpssc {

RulesTempVar::RulesTempVar(const std::string &name) :
	FieldImpl(name, 0, IField::FieldAttr_None, 0),
	RulesItem(RulesItem_TempVar) {

	setType(IBaseItem::TypeVendor);
}

RulesTempVar::~RulesTempVar() {
	// TODO Auto-generated destructor stub
}

} /* namespace qpssc */
