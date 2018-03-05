/*
 * TypeExtensionProcessor.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#include "TypeExtensionProcessor.h"
#include <stdio.h>

namespace qpssc {

TypeExtensionProcessor::TypeExtensionProcessor() {
	// TODO Auto-generated constructor stub

}

TypeExtensionProcessor::~TypeExtensionProcessor() {
	// TODO Auto-generated destructor stub
}

bool TypeExtensionProcessor::process(IModel *model) {
	visit_model(model);

	return true;
}

void TypeExtensionProcessor::visit_package(IPackage *pkg) {
	if (pkg->getName() == "" || pkg->hasAttribute("REFERENCED")) {
		fprintf(stdout, "Visiting package %s\n", pkg->getName().c_str());
		PSIVisitor::visit_package(pkg);
	}
}

void TypeExtensionProcessor::visit_extend(IExtend *e) {
	// Add content from the extension to the target element
	IScopeItem *target_s = dynamic_cast<IScopeItem *>(e->getTarget());

	fprintf(stdout, "visit_extend\n");

	for (std::vector<IBaseItem *>::const_iterator it=e->getItems().begin();
			it!=e->getItems().end(); it++) {
		IBaseItem *item_c = (*it)->clone();
		// TODO: need clone these elements and prune the extension later
		// TODO: for now, just create a duplicate reference. This is a
		// problem because the 'parent' ref will be incorrect.
		fprintf(stdout, "item_c=%p\n", item_c);
		target_s->add(item_c);
		fprintf(stdout, "item_c.parent=%p\n", item_c->getParent());
	}
}

} /* namespace qpssc */
