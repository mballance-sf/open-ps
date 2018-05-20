/*
 * Z3ModelEvaluator.cpp
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#include "Z3ModelEvaluator.h"
#include "Z3Model.h"
#include "Z3ModelEvalAction.h"

Z3ModelEvaluator::Z3ModelEvaluator(Z3Model *model) {
	m_model = model;
	m_eval_action = new Z3ModelEvalAction(this);

}

Z3ModelEvaluator::~Z3ModelEvaluator() {
	// TODO Auto-generated destructor stub
}

