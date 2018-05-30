/*
 * Z3ModelFindSelectVars.cpp
 *
 *  Created on: May 29, 2018
 *      Author: ballance
 */

#include "Z3ModelFindSelectVars.h"

Z3ModelFindSelectVars::Z3ModelFindSelectVars(
		const Z3Model 			&model,
		ModelNameProvider		&name_provider
		) : m_model(model), m_name_provider(name_provider), m_vars(0) {
	// TODO Auto-generated constructor stub

}

Z3ModelFindSelectVars::~Z3ModelFindSelectVars() {
	// TODO Auto-generated destructor stub
}

void Z3ModelFindSelectVars::find(
		std::vector<Z3ModelVar *>		&vars,
		IActivityStmt					*stmt) {
	m_vars = &vars;
	visit_activity_stmt(stmt);
}

void Z3ModelFindSelectVars::visit_activity_select_stmt(IActivitySelectStmt *s) {

}
