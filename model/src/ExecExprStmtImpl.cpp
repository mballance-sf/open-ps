/*
 * ExecExprStmtImpl.cpp
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

#include "ExecExprStmtImpl.h"

namespace psi {

ExecExprStmtImpl::ExecExprStmtImpl(
		IExpr						*lhs,
		IExecExprStmt::AssignOp		op,
		IExpr						*rhs) :
				ExecStmtImpl(IExecStmt::StmtType_Expr),
				m_lhs(lhs), m_op(op), m_rhs(rhs) {
	// TODO Auto-generated constructor stub

}

ExecExprStmtImpl::~ExecExprStmtImpl() {
	// TODO Auto-generated destructor stub
}

IExecStmt *ExecExprStmtImpl::clone() const {
	return new ExecExprStmtImpl(
			getLhs()->clone(),
			getOp(),
			(getRhs())?getRhs()->clone():0);
}

} /* namespace psi */
