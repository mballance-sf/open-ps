/*
 * IExecExprStmt.h
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
 *  Created on: Aug 30, 2016
 *      Author: ballance
 */

#pragma once
#include "IExecStmt.h"

class IExecExprStmt : public virtual IExecStmt {
public:
	enum AssignOp {
		AssignOp_None, // no assignment
		AssignOp_Eq,
		AssignOp_AndEq,
		AssignOp_OrEq,
		AssignOp_PlusEq,
		AssignOp_MinusEq,
		AssignOp_LShiftEq,
		AssignOp_RShiftEq
	};

	virtual ~IExecExprStmt() { }

	virtual IExpr *getLhs() const = 0;

	virtual IExecExprStmt::AssignOp getOp() const = 0;

	virtual IExpr *getRhs() const = 0;

};


