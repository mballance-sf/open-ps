/*
 * Z3ModelEvaluator.h
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
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include "Z3Model.h"
#include "Z3ModelEvalAction.h"
#include "Z3ModelEvalVarInit.h"
#include "IExecListener.h"
#include "ModelNameProvider.h"
#include "Z3ModelEvalVarInit.h"

class Z3ModelEvaluator {
public:

	Z3ModelEvaluator(const Z3ModelH &model);

	virtual ~Z3ModelEvaluator();

	void eval(IExecListener *l);

	Z3ModelEvalAction &action_evaluator() { return m_eval_action; }

	Z3ModelEvalVarInit &var_initializer() { return m_var_initializer; }

	IExecListener *exec_listener() const { return m_exec_listener; }

	ModelNameProvider &name_provider() { return m_name_provider; }

	Z3Model *model() { return m_model.get(); }

	Z3ModelVar *get_variable(const std::string &name);

	const VarVal &get_val(Z3ModelVar *var);

	bool solve(const std::vector<Z3ModelVar *> &vars);

	bool solve(Z3ModelVar *var);

private:
	Z3ModelH					m_model;
	Z3ModelEvalAction			m_eval_action;
	Z3ModelEvalVarInit			m_var_initializer;
	ModelNameProvider			m_name_provider;
	IExecListener				*m_exec_listener;

};

