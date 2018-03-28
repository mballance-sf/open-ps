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

//	enum Type {
//		TypeSet,
//		TypeBit
//	};

	Z3ModelVar(
			const std::string		&name,
			Z3_ast					m_var,
			uint32_t				m_bits);

	virtual ~Z3ModelVar();

	const std::string &name() const { return m_name; }

	Z3_ast var() const { return m_var; }

	uint32_t bits() const { return m_bits; }

private:
	std::string			m_name;
	Z3_ast				m_var;
	uint32_t			m_bits;
};

#endif /* PROCESSOR_SRC_Z3MODELVAR_H_ */
