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
#include "VarVal.h"

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
			VarValType				type);

	virtual ~Z3ModelVar();

	const std::string &name() const { return m_name; }

	Z3_ast var() const { return m_var; }

	uint32_t bits() const { return m_bits; }

	bool is_signed() const { return m_val.type == VarVal_Int; }

	// Always returns the integral value of the variable
	const VarVal &get_val(
			Z3_context 		c,
			Z3_model 		m);

	void set_val(uint64_t v) {
		m_val.ui = v;
		m_fixed = true;
		m_val_valid = true;
	}

	void invalidate() { m_val_valid = false; }

	bool fixed() const { return m_fixed; }

	void reset() {
		m_val_valid = false;
		m_fixed = false;
	}

private:
	std::string			m_name;
	Z3_ast				m_var;
	uint32_t			m_bits;
	bool				m_val_valid;
	bool				m_fixed;
	VarVal				m_val;
};

#endif /* PROCESSOR_SRC_Z3MODELVAR_H_ */
