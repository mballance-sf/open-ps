/*
 * Z3ModelEvalAction.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once
#include "IAction.h"
#include <string>

class Z3ModelEvaluator;
class Z3ModelEvalAction {
public:
	Z3ModelEvalAction(Z3ModelEvaluator *evaluator);

	virtual ~Z3ModelEvalAction();

	void eval_action(
			const std::string		&context,
			IAction 				*action);

private:
	Z3ModelEvaluator			*m_evaluator;


};

