/*
 * RulesPSIVisitor.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESPSIVISITOR_H_
#define SRC_RULESPSIVISITOR_H_

#include "PSIVisitor.h"
#include "RulesActionItem.h"
#include "RulesExecActionItem.h"
#include "RulesExecStmtItem.h"
#include "RulesExecStmtIfItem.h"
#include "RulesTempVar.h"

namespace qpssc {

class RulesPSIVisitor: public virtual psi::apps::PSIVisitor {
public:
	RulesPSIVisitor();
	virtual ~RulesPSIVisitor();

	virtual void visit_exec_vendor_stmt(IExecStmt *s);

	virtual void visit_rules_exec_if(RulesExecStmtIfItem *s);

	virtual void visit_rules_action_exec_item(RulesExecActionItem *it);

	virtual void visit_rules_temp_var(RulesTempVar *it);

	virtual void visit_vendor_item(IBaseItem *it);

};

} /* namespace qpssc */

#endif /* SRC_RULESPSIVISITOR_H_ */
