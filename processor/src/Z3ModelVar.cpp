/*
 * Z3ModelVar.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#include "Z3ModelVar.h"

Z3ModelVar::Z3ModelVar(
		const std::string		&name,
		Z3_ast					var) {
	m_name = name;
	m_var = var;
}

Z3ModelVar::~Z3ModelVar() {
	// TODO Auto-generated destructor stub
}

