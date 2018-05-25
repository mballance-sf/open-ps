/*
 * Z3ModelVar.h
 *
 * Licensed under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in
 *  compliance with the License.  You may obtain a copy of
 *  the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See
 * the License for the specific language governing
 * permissions and limitations under the License.
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
