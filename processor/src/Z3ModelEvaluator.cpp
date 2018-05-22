/*
 * Z3ModelEvaluator.cpp
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
	m_eval_action.eval_action(entry->getName(), entry);
}


