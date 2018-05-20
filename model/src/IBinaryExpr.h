/*
 * IBinaryExpr.h
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
 *  Created on: May 4, 2016
 *      Author: ballance
 */

#pragma once

#include "IExpr.h"

class IBinaryExpr : public IExpr {

public:

	enum BinOpType {
		BinOp_Eq, 		// 0
		BinOp_EqEq,
		BinOp_NotEq,
		BinOp_GE,
		BinOp_GT,
		BinOp_LE,		// 5
		BinOp_LT,
		BinOp_And,
		BinOp_AndAnd,
		BinOp_Or,
		BinOp_OrOr,
		BinOp_Minus,
		BinOp_Plus,
		BinOp_Multiply,
		BinOp_Divide,
		BinOp_Mod,
		BinOp_ArrayRef
	};

public:

	virtual ~IBinaryExpr() { }

	virtual BinOpType getBinOpType() const = 0;

	virtual IExpr *getLHS() const = 0;

	virtual IExpr *getRHS() const = 0;

};




