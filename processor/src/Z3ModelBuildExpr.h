/*
 * Z3ModelBuildExpr.h
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
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "ModelVisitor.h"
#include "Z3ExprTerm.h"



class Z3ModelBuilder;

class Z3ModelBuildExpr : public virtual ModelVisitor {
public:
	Z3ModelBuildExpr(Z3ModelBuilder *builder);

	virtual ~Z3ModelBuildExpr();

	Z3ExprTerm build(IExpr *expr);

	virtual void visit_binary_expr(IBinaryExpr *be) override;

	virtual void visit_in_expr(IInExpr *in);

	virtual void visit_literal_expr(ILiteral *l) override;

	virtual void visit_variable_ref(IVariableRef *ref) override;

	Z3ExprTerm mk_bitwise_and(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_logical_and(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_bitwise_or(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_logical_or(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_eq(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_ge(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_gt(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_le(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_lt(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm mk_bool(bool v);

	Z3ExprTerm mk_int(int32_t v, uint32_t bits=32);

	Z3ExprTerm mk_var(Z3ModelVar *var);

	Z3ExprTerm upsize(const Z3ExprTerm &target, uint32_t bits);

private:

	void size_terms(Z3ExprTerm &lhs, Z3ExprTerm &rhs);

	Z3ExprTerm size_lhs(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

	Z3ExprTerm size_rhs(const Z3ExprTerm &lhs, const Z3ExprTerm &rhs);

private:
	Z3ModelBuilder					*m_builder;
	Z3ExprTerm						m_expr;

};


