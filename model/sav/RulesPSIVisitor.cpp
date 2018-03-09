/*
 * RulesPSIVisitor.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "RulesPSIVisitor.h"
#include <stdio.h>

namespace qpssc {

RulesPSIVisitor::RulesPSIVisitor() { }

RulesPSIVisitor::~RulesPSIVisitor() { }

void RulesPSIVisitor::visit_exec_vendor_stmt(IExecStmt *s) {
	RulesExecStmtItem *rs = dynamic_cast<RulesExecStmtItem *>(s);

	switch (rs->getRulesStmtType()) {
	case RulesExecStmtItem::If:
		visit_rules_exec_if(dynamic_cast<RulesExecStmtIfItem *>(s));
		break;
	default:
		fprintf(stdout, "Error: Unknown rules exec stmt %d\n", rs->getStmtType());
	}
}

void RulesPSIVisitor::visit_rules_exec_if(RulesExecStmtIfItem *s) {
	// NOP
}

void RulesPSIVisitor::visit_rules_action_exec_item(RulesExecActionItem *it) {
	// NOP
}

void RulesPSIVisitor::visit_rules_temp_var(RulesTempVar *it) {
	// NOP
}

void RulesPSIVisitor::visit_vendor_item(IBaseItem *it) {
	RulesItem *r_it = dynamic_cast<RulesItem *>(it);

	switch (r_it->getItemType()) {
	case RulesItem::RulesItem_ActionExecItem:
		visit_rules_action_exec_item(dynamic_cast<RulesExecActionItem *>(it));
		break;

	case RulesItem::RulesItem_TempVar:
		visit_rules_temp_var(dynamic_cast<RulesTempVar *>(it));
		break;

	default:
		fprintf(stdout, "Error: Unknown rules item type %d\n", r_it->getItemType());
	}
}

} /* namespace qpssc */
