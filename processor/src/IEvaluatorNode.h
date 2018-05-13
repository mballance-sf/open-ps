/*
 * IEvaluatorNode.h
 *
 *  Created on: Apr 29, 2018
 *      Author: ballance
 */
#pragma once

class IEvaluatorNode {
public:

	virtual ~IEvaluatorNode() { }

	virtual bool eval() = 0;

};




