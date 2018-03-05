/*
 * ActivityInferrencingProcessor.cpp
 *
 *  Created on: Jan 20, 2017
 *      Author: ballance
 */
#include <stdio.h>
#include "ActivityInferrencingProcessor.h"
#include "ComponentTreeNode.h"
#include "QPSSCUtils.h"

namespace qpssc {

ActivityInferrencingProcessor::ActivityInferrencingProcessor() {
	// TODO Auto-generated constructor stub

}

ActivityInferrencingProcessor::~ActivityInferrencingProcessor() {
	// TODO Auto-generated destructor stub
}

bool ActivityInferrencingProcessor::process(IModel *model) {
	IComponent *comp;
	IAction *action;

	if (!QPSSCUtils::get_roots(model, &comp, &action)) {
		return false;
	}

	ComponentTreeNode *tree = ComponentTreeNode::build(model, comp);

	fprintf(stdout, "Component Tree:\n%s", tree->toString().c_str());

	return true;
}

} /* namespace qpssc */
