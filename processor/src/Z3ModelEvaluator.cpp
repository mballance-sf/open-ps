/*
 * Z3ModelEvaluator.cpp
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

#include "Z3ModelEvaluator.h"
#include "Z3Model.h"
#include "Z3ModelEvalAction.h"

Z3ModelEvaluator::Z3ModelEvaluator(const Z3ModelH &model) :
	m_model(model),
	m_eval_action(this),
	m_var_initializer(this),
	m_exec_listener(0) {

}

Z3ModelEvaluator::~Z3ModelEvaluator() {
	// TODO Auto-generated destructor stub
}

void Z3ModelEvaluator::eval(IExecListener *l) {
	m_exec_listener = l;

	// Clear all variables
	m_model->init();

	IAction *entry = m_model->entry();
	m_name_provider.enter(entry->getName());
	m_eval_action.eval_action(entry);
	m_name_provider.leave(entry->getName());
}

Z3ModelVar *Z3ModelEvaluator::get_variable(const std::string &name) {
	return m_model->get_variable(name);
}

const VarVal &Z3ModelEvaluator::get_val(Z3ModelVar *var) {
	return m_model->get_val(var);
}

bool Z3ModelEvaluator::solve(const std::vector<Z3ModelVar *> &vars) {
	return m_model->solve(vars);
}
