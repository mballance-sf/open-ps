/*
 * Z3ModelProcessor.h
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
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */
#pragma once

#include <string>
#include <vector>
#include <map>
#include "Z3Model.h"
#include "Z3ModelVar.h"
#include "Z3ExprTerm.h"
#include "ModelVisitor.h"
#include "StringTableBuilder.h"
#include "SolverErrorException.h"
#include "IVarValueProvider.h"
#include "IExecListener.h"
#include "LFSR.h"
#include "z3.h"



class Z3ModelProcessor {
public:

	Z3ModelProcessor();

	virtual ~Z3ModelProcessor();

	virtual bool build(
			IComponent 		*comp,
			IAction			*action);

	virtual bool run();

	void set_exec_listener(IExecListener *l);

protected:


private:

	void exec_action(
			const std::string				&context,
			IAction 						*action);

	void collect_variables(
			std::vector<Z3ModelVar *>		&vars,
			const std::string				&context,
			IAction							*action);

	void collect_rand_variables(
			std::vector<Z3ModelVar *>		&vars,
			const std::string				&context,
			IAction							*action);

	void exec_activity_stmt(
			const std::string				&context,
			IActivityStmt					*stmt);

	void exec_activity_traverse_stmt(
			const std::string				&context,
			IActivityTraverseStmt			*stmt);

	void exec_activity_repeat_stmt(
			const std::string				&context,
			IActivityRepeatStmt				*repeat);


private:
	IAction									*m_root_action;
	IComponent								*m_root_comp;
	Z3Model									*m_model;

	uint32_t								m_expr_depth;
	IExecListener							*m_exec_listener;

};

