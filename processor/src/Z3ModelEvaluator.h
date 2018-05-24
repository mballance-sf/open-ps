/*
 * Z3ModelEvaluator.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include "Z3Model.h"
#include "Z3ModelEvalAction.h"
#include "IExecListener.h"
#include "ModelNameProvider.h"

class Z3ModelEvaluator {
public:

	Z3ModelEvaluator(const Z3ModelH &model);

	virtual ~Z3ModelEvaluator();

	void eval(IExecListener *l);

	Z3ModelEvalAction &action_evaluator() { return m_eval_action; }

	IExecListener *exec_listener() const { return m_exec_listener; }

	ModelNameProvider &name_provider() { return m_name_provider; }

	Z3ModelVar *get_variable(const std::string &name);

	const VarVal &get_val(Z3ModelVar *var);

	bool solve(const std::vector<Z3ModelVar *> &vars);

private:
	Z3ModelH					m_model;
	Z3ModelEvalAction			m_eval_action;
	ModelNameProvider			m_name_provider;
	IExecListener				*m_exec_listener;

};

