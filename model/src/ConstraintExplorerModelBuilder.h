/*
 * ConstraintExplorerModelBuilder.h
 *
 *  Created on: Dec 14, 2017
 *      Author: ballance
 */

#ifndef QPSSC_SRC_CONSTRAINTEXPLORERMODELBUILDER_H_
#define QPSSC_SRC_CONSTRAINTEXPLORERMODELBUILDER_H_

#include "RulesPSIVisitor.h"
#include <vector>
#include <string>
#include <map>

namespace ConstraintExplorer {
class Session;
class Variable;
}

class ConstraintExplorerModelBuilder: public qpssc::RulesPSIVisitor {
public:
	ConstraintExplorerModelBuilder();

	virtual ~ConstraintExplorerModelBuilder();

	// TODO: may need
	virtual void visit(
			IModel		*model,
			IAction		*root);

	virtual void visit_action(
			const std::string		&prefix,
			IAction					*action);

private:

private:
	ConstraintExplorer::Session								*m_session;
	std::map<std::string, ConstraintExplorer::Variable *>	m_var_map;
	std::vector<std::string>								m_prefix_stack;

};

#endif /* QPSSC_SRC_CONSTRAINTEXPLORERMODELBUILDER_H_ */
