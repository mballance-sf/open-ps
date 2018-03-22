/*
 * ActivityTraverseStmtImpl.h
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
 * 
 *  Created on: Jun 10, 2016
 *      Author: ballance
 */

#ifndef INCLUDED_ACTIVITY_TRAVSERSE_STMT_H
#define INCLUDED_ACTIVITY_TRAVSERSE_STMT_H
#include "IActivityTraverseStmt.h"

using namespace psi_api;

namespace psi {

class ActivityTraverseStmtImpl: public IActivityTraverseStmt {
public:
	ActivityTraverseStmtImpl(
			IVariableRef	*action,
			IConstraint 	*c);

	virtual ~ActivityTraverseStmtImpl();

	virtual GraphStmtType getStmtType() const {
		return GraphStmt_Traverse;
	}

	virtual IVariableRef *getAction() const {
		return m_action;
	}

	virtual IConstraint *getWith() const {
		return m_constraint;
	}

	virtual IGraphStmt *clone() const;

private:

	IVariableRef				*m_action;
	IConstraint					*m_constraint;

};

} /* namespace psi */

#endif /* INCLUDED_ACTIVITY_TRAVSERSE_STMT_H */
