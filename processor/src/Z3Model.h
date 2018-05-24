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
#include "IStringTable.h"
#include "IAction.h"
#include "z3.h"
#include <memory>

class Z3Model : public virtual IVarValueProvider {

public:
	Z3Model(
			const IStringTableH &strtab,
			IAction				*entry);

	virtual ~Z3Model();

	void init();

	Z3_context ctxt() const { return m_ctxt; }

	Z3_solver solver() const { return m_solver; }

	const VarVal &get_val(Z3ModelVar *var);

	std::string toString();

	bool solve(const std::vector<Z3ModelVar *> &vars);

	bool check();

	void add_variable(Z3ModelVar *var);

	Z3ModelVar *get_variable(const std::string &name);

	virtual VarVal get_value(const std::string &path);

	IAction *entry() const { return m_entry; }

private:
	static void z3_error_handler(Z3_context c, Z3_error_code e);

private:
	Z3_config								m_cfg;
	Z3_context								m_ctxt;
	Z3_model								m_model;
	Z3_solver								m_solver;
	IStringTableH							m_strtab;
	IAction									*m_entry;
	std::map<std::string, Z3ModelVar *>		m_variables;
	LFSR									m_lfsr;

};

typedef std::shared_ptr<Z3Model> Z3ModelH;

