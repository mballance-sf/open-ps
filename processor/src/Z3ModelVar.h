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
			Z3_ast					var,
			uint32_t				bits,
			bool					is_signed);

	virtual ~Z3ModelVar();

	const std::string &name() const { return m_name; }

	Z3_ast var() const { return m_var; }

	uint32_t bits() const { return m_bits; }

	bool is_signed() const { return m_is_signed; }

	uint64_t get_val(Z3_context c, Z3_model m);

	void reset() { m_val_valid = false; }

private:
	std::string			m_name;
	Z3_ast				m_var;
	uint32_t			m_bits;
	bool				m_is_signed;
	uint64_t			m_val;
	bool				m_val_valid;
};

#endif /* PROCESSOR_SRC_Z3MODELVAR_H_ */
