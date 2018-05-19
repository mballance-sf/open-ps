/*
 * Z3ModelVar.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: ballance
 */

#include "Z3ModelVar.h"

Z3ModelVar::Z3ModelVar(
		const std::string		&name,
		Z3_ast					var,
		uint32_t				bits,
		VarValType				type) {
	m_name = name;
	m_var = var;
	m_bits = bits;
	m_val.type = type;
	m_val.ui = 0;
	m_val_valid = false;
	m_fixed = false;
}

Z3ModelVar::~Z3ModelVar() {
	// TODO Auto-generated destructor stub
}

const VarVal &Z3ModelVar::get_val(
		Z3_context 	c,
		Z3_model 	m) {
	if (!m_val_valid) {
		Z3_ast v_ast;
		if (c && m) {
		Z3_model_eval(c, m, m_var, true, &v_ast);
		Z3_get_numeral_uint64(c, v_ast,
				(__uint64 *)&m_val.ui);
		m_val_valid = true;
		} else {
			fprintf(stdout, "Error: attempting to fetch unresolved var %s\n",
					m_name.c_str());
		}
	}
	return m_val;
}

