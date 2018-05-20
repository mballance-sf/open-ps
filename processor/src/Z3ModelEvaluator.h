/*
 * Z3ModelEvaluator.h
 *
 *  Created on: May 20, 2018
 *      Author: ballance
 */

#pragma once

class Z3Model;
class Z3ModelEvalAction;
class Z3ModelEvaluator {
public:

	Z3ModelEvaluator(Z3Model *model);

	virtual ~Z3ModelEvaluator();

private:
	Z3Model						*m_model;
	Z3ModelEvalAction			*m_eval_action;

};

