/*
 * RulesExecActionItem.h
 *
 *  Created on: Nov 2, 2016
 *      Author: ballance
 */

#ifndef SRC_RULESEXECACTIONITEM_H_
#define SRC_RULESEXECACTIONITEM_H_

#include "FieldImpl.h"

#include "RulesItem.h"
#include "ExecImpl.h"

namespace qpssc {

class RulesExecActionItem :
		public FieldImpl,
		public virtual RulesItem {
public:
	RulesExecActionItem(const std::string &name);

	virtual ~RulesExecActionItem();

	ExecImpl *getExec();

private:
	ExecImpl					m_exec;


};

} /* namespace qpssc */

#endif /* SRC_RULESEXECACTIONITEM_H_ */
