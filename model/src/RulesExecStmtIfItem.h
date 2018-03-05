/*
 * RulesExecStmtIfItem.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESEXECSTMTIFITEM_H_
#define SRC_RULESEXECSTMTIFITEM_H_

#include "RulesExecStmtItem.h"
#include "BaseItemImpl.h"
#include <vector>

namespace qpssc {

class RulesExecStmtIfItem: public RulesExecStmtItem {
public:
	RulesExecStmtIfItem(
			IExpr							*cond,
			const std::vector<IExecStmt *>	&stmt_true,
			const std::vector<IExecStmt *>	&stmt_false);

#ifdef PSS_HAVE_CXX_11
	RulesExecStmtIfItem(
			IExpr								*cond,
			std::initializer_list<IExecStmt *>	stmt_true,
			std::initializer_list<IExecStmt *>	stmt_false) :
				RulesExecStmtIfItem(cond,
						std::vector<IExecStmt *>(stmt_true),
						std::vector<IExecStmt *>(stmt_false)) { }

#endif

	virtual ~RulesExecStmtIfItem();

	IExpr *getCond() const;

	const std::vector<IExecStmt *> &getTrue() const;

	const std::vector<IExecStmt *> &getFalse() const;

	virtual IExecStmt *clone() const;


private:
	IExpr						*m_cond;
	std::vector<IExecStmt *>	m_true;
	std::vector<IExecStmt *>	m_false;
};

} /* namespace qpssc */

#endif /* SRC_RULESEXECSTMTIFITEM_H_ */
