/*
 * Z3ModelEvalVarInit.cpp
 *
 *  Created on: May 23, 2018
 *      Author: ballance
 */

#include "Z3ModelEvalVarInit.h"

Z3ModelEvalVarInit::Z3ModelEvalVarInit(Z3ModelEvaluator *evaluator) {
	m_evaluator = evaluator;
}

Z3ModelEvalVarInit::~Z3ModelEvalVarInit() {
	// TODO Auto-generated destructor stub
}

void Z3ModelEvalVarInit::init(IAction *action) {

	if (action->getActivity()) {
		visit_activity(action->getActivity());
	}
}
