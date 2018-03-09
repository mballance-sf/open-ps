/*
 * ConstraintExplorerModelBuilder.cpp
 *
 *  Created on: Dec 14, 2017
 *      Author: ballance
 */

#include "ConstraintExplorerModelBuilder.h"

ConstraintExplorerModelBuilder::ConstraintExplorerModelBuilder() {
	// TODO Auto-generated constructor stub

}

ConstraintExplorerModelBuilder::~ConstraintExplorerModelBuilder() {
	// TODO Auto-generated destructor stub
}

void ConstraintExplorerModelBuilder::visit(
		IModel				*model,
		IAction				*root) {

	m_prefix_stack.clear();
	m_prefix_stack.push_back(root->getName());
	RulesPSIVisitor::visit_action(root);

}

