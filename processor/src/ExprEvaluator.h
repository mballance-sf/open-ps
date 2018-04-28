/*
 * ExprEvaluator.h
 *
 *  Created on: Apr 24, 2018
 *      Author: ballance
 */
#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "IExpr.h"
#include "IVarValueProvider.h"
#include "PSIVisitor.h"

using namespace psi_api;
using namespace psi::apps;

class ExprEvaluator : public virtual PSIVisitor {
public:


public:
	ExprEvaluator(IVarValueProvider *var_provider);

	virtual ~ExprEvaluator();

	const VarVal &eval(const std::string &context, IExpr *expr);

protected:
	virtual void visit_binary_expr(IBinaryExpr *be);

	virtual void visit_literal_expr(ILiteral *l);

	virtual void visit_variable_ref(IVariableRef *ref);

	std::string context();

private:
	VarVal							m_val;
	IVarValueProvider				*m_var_provider;
	std::vector<std::string>		m_context;

};

