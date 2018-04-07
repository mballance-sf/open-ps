/*
 * IActivityStmt.h
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
 *
 *  Created on: May 8, 2016
 *      Author: ballance
 */
#pragma once
#include "IBaseItem.h"

namespace psi_api {

class IActivityStmt : public virtual IBaseItem {
public:
	enum ActivityStmtType {
		/*! activity block statement. Object is of type IActivityBlockStmt. */
		ActivityStmt_Block,
		/*! activity if/else statement. Object is of type IActivityIfElseStmt.
		 * Used for both top-level if/else and for if-gated pss_select statement */
		ActivityStmt_IfElse,
		/*! activity parallel statement. Object is of type IActivityBlockStmt */
		ActivityStmt_Parallel,
		/*! activity schedule statement. Object is of type IActivityBlockStmt */
		ActivityStmt_Schedule,
		//! activity pss_select statement. Object is of type IActivityBlockStmt
		ActivityStmt_Select,
		//! activity repeat, repeat while, or repeat ... while statement.
		//! Object is of type IActivityRepeatStmt
		ActivityStmt_Repeat,

		/*! activity action-traversal statement */
		ActivityStmt_Traverse,
		ActivityStmt_DoAction
	};

public:
	virtual ~IActivityStmt() { }

	virtual ActivityStmtType getStmtType() const = 0;

	virtual IActivityStmt *clone() const = 0;

};
}


