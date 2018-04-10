/*
 * ActivityRepeatStmtImpl.h
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

#pragma once
#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "IActivityRepeatStmt.h"

using namespace psi_api;

namespace psi {

class ActivityRepeatStmtImpl:
		public virtual BaseItemImpl,
		public virtual ScopeItemImpl,
		public virtual IActivityRepeatStmt {
public:
	ActivityRepeatStmtImpl(RepeatType type, IExpr *expr, IActivityStmt *body);

	virtual ~ActivityRepeatStmtImpl();

	virtual ActivityStmtType getStmtType() const { return ActivityStmt_Repeat; }

	virtual RepeatType getRepeatType() const { return m_type; }

	virtual IExpr *getCond() const { return m_cond; }

	virtual IActivityStmt *getBody() const { return m_body; }

	virtual void setBody(IActivityStmt *s);

	virtual IActivityStmt *clone() const;

private:
	RepeatType				m_type;
	IExpr					*m_cond;
	IActivityStmt			*m_body;
};


} /* namespace psi */

