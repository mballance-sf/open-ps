/*
 * RulesExecStmtItem.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESEXECSTMTITEM_H_
#define SRC_RULESEXECSTMTITEM_H_

#include <ExecStmtImpl.h>

namespace qpssc {

class RulesExecStmtItem: public psi::ExecStmtImpl {
public:
	enum RulesExecStmtType {
		If
	};
	RulesExecStmtItem(RulesExecStmtType t);

	virtual ~RulesExecStmtItem();

	RulesExecStmtItem::RulesExecStmtType getRulesStmtType();

private:
	RulesExecStmtType				m_stmt_type;

};

} /* namespace qpssc */

#endif /* SRC_RULESEXECSTMTITEM_H_ */
