/*
 * Z3ModelEvalVarInit.h
 *
 *  Created on: May 23, 2018
 *      Author: ballance
 */

#pragma once
#include "PSIVisitor.h"

class Z3ModelEvaluator;
class Z3ModelEvalVarInit : public virtual PSIVisitor {
public:
	Z3ModelEvalVarInit(Z3ModelEvaluator *evaluator);

	virtual ~Z3ModelEvalVarInit();

	void init(IAction *action);

	void init(IActivityStmt *stmt);

private:

	Z3ModelEvaluator				*m_evaluator;

};

