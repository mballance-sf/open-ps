/*
 * Z3SolveContext.h
 *
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_Z3SOLVECONTEXT_H_
#define PROCESSOR_SRC_Z3SOLVECONTEXT_H_
#include <map>
#include <string>
#include <vector>
#include "z3.h"

class Z3SolveContext {
public:

	Z3SolveContext();

	virtual ~Z3SolveContext();

private:
	std::map<std::string, Z3_ast>		m_variables;

};

#endif /* PROCESSOR_SRC_Z3SOLVECONTEXT_H_ */
