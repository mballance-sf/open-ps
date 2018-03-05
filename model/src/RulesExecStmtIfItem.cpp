/*
 * RulesExecStmtIfItem.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#include "RulesExecStmtIfItem.h"

namespace qpssc {

RulesExecStmtIfItem::RulesExecStmtIfItem(
		IExpr							*cond,
		const std::vector<IExecStmt *>	&stmt_true,
		const std::vector<IExecStmt *>	&stmt_false) :
				RulesExecStmtItem(RulesExecStmtItem::If),
				m_cond(cond),
				m_true(stmt_true),
				m_false(stmt_false) {
}

RulesExecStmtIfItem::~RulesExecStmtIfItem() {
	// TODO Auto-generated destructor stub
}

IExpr *RulesExecStmtIfItem::getCond() const {
	return m_cond;
}

const std::vector<IExecStmt *> &RulesExecStmtIfItem::getTrue() const {
	return m_true;
}

const std::vector<IExecStmt *> &RulesExecStmtIfItem::getFalse() const {
	return m_false;
}

IExecStmt *RulesExecStmtIfItem::clone() const {
	std::vector<IExecStmt *> stmt_true, stmt_false;

	for (std::vector<IExecStmt *>::const_iterator it=getTrue().begin();
			it!=getTrue().end(); it++) {
		stmt_true.push_back((*it)->clone());
	}
	for (std::vector<IExecStmt *>::const_iterator it=getFalse().begin();
			it!=getFalse().end(); it++) {
		stmt_false.push_back((*it)->clone());
	}

	return new RulesExecStmtIfItem(
			getCond()->clone(),
			stmt_true,
			stmt_false);
}


} /* namespace qpssc */
