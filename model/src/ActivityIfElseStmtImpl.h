/*
 * ActivityIfElseStmtImpl.h
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
 *  Created on: Apr 7, 2018
 *      Author: ballance
 */

#pragma once

#include "BaseItemImpl.h"
#include "IActivityIfElseStmt.h"



class ActivityIfElseStmtImpl:
		public virtual BaseItemImpl,
		public virtual IActivityIfElseStmt {
public:
	ActivityIfElseStmtImpl(
			IExpr			*cond,
			IActivityStmt	*true_stmt,
			IActivityStmt	*false_stmt);

	virtual ~ActivityIfElseStmtImpl();

	virtual ActivityStmtType getStmtType() const {
		return IActivityStmt::ActivityStmt_IfElse;
	}

	virtual IExpr *getCond() const { return m_cond; }

	virtual IActivityStmt *getTrue() const { return m_true; }

	virtual IActivityStmt *getFalse() const { return m_false; }

	virtual IActivityStmt *clone() const;

private:
	IExpr				*m_cond;
	IActivityStmt		*m_true;
	IActivityStmt		*m_false;
};

