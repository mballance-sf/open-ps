/*
 * RulesExecStmtItem.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "RulesExecStmtItem.h"

namespace qpssc {

RulesExecStmtItem::RulesExecStmtItem(RulesExecStmtType t) :
			ExecStmtImpl(IExecStmt::StmtType_Vendor), m_stmt_type(t) {

}

RulesExecStmtItem::~RulesExecStmtItem() {
	// TODO Auto-generated destructor stub
}

RulesExecStmtItem::RulesExecStmtType RulesExecStmtItem::getRulesStmtType() {
	return m_stmt_type;
}

} /* namespace qpssc */
