/*
 * Z3Model.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */
#pragma once

#include <map>
#include <string>
#include <vector>
#include "Z3ModelVar.h"
#include "LFSR.h"
#include "IVarValueProvider.h"
#include "StringTableBuilder.h"
#include "z3.h"

class Z3Model : public StringTableBuilder,
	public virtual IVarValueProvider {

public:
	Z3Model();

	virtual ~Z3Model();

	bool solve(const std::vector<Z3ModelVar *> &vars);

	bool check();

	virtual VarVal get_value(const std::string &path);

private:
	static void z3_error_handler(Z3_context c, Z3_error_code e);

private:
	Z3_config								m_cfg;
	Z3_context								m_ctxt;
	Z3_model								m_model;
	Z3_solver								m_solver;
	std::map<std::string, Z3ModelVar *>		m_variables;
	LFSR									m_lfsr;

};

