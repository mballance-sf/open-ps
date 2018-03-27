/*
 * Z3ModelVar.h
 *
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_Z3MODELVAR_H_
#define PROCESSOR_SRC_Z3MODELVAR_H_
#include <string>
#include "z3.h"

class Z3ModelVar {
public:

	Z3ModelVar(
			const std::string		&name,
			Z3_ast					m_var);

	virtual ~Z3ModelVar();

	const std::string &name() const { return m_name; }

	Z3_ast var() const { return m_var; }

private:
	std::string			m_name;
	Z3_ast				m_var;
};

#endif /* PROCESSOR_SRC_Z3MODELVAR_H_ */
