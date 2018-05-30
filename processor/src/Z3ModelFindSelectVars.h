/*
 * Z3ModelFindSelectVars.h
 *
 *  Created on: May 29, 2018
 *      Author: ballance
 */
#include "ModelVisitor.h"
#include "Z3Model.h"
#include "ModelNameProvider.h"
#include <vector>

class Z3ModelFindSelectVars : public virtual ModelVisitor {
public:
	Z3ModelFindSelectVars(
			const Z3Model 		&model,
			ModelNameProvider	&name_provider);

	virtual ~Z3ModelFindSelectVars();

	void find(
			std::vector<Z3ModelVar *> &vars,
			IActivityStmt			*stmt);

protected:

	virtual void visit_activity_select_stmt(IActivitySelectStmt *s) override;

private:
	const Z3Model				&m_model;
	ModelNameProvider			&m_name_provider;
	std::vector<Z3ModelVar *>	*m_vars;
};

