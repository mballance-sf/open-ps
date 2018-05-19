/*
 * Z3ModelExprBuilder.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "PSIVisitor.h"
#include "Z3ExprTerm.h"

using namespace psi::apps;

class Z3ModelBuilder;

class Z3ModelExprBuilder : public virtual PSIVisitor {
public:
	Z3ModelExprBuilder(Z3ModelBuilder *builder);

	virtual ~Z3ModelExprBuilder();

	Z3ExprTerm build(IExpr *expr);

	virtual void visit_binary_expr(IBinaryExpr *be) override;

	virtual void visit_literal_expr(ILiteral *l) override;

	virtual void visit_variable_ref(IVariableRef *ref) override;

	Z3ExprTerm upsize(const Z3ExprTerm &target, uint32_t bits);

private:
	Z3ModelBuilder					*m_builder;
	Z3ExprTerm						m_expr;

};


