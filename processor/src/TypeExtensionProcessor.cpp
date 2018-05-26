/*
 * TypeExtensionProcessor.cpp
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
		ModelVisitor::visit_package(pkg);
	}
}

void TypeExtensionProcessor::visit_extend_composite(IExtendComposite *e) {
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

void TypeExtensionProcessor::visit_extend_enum(IExtendEnum *e) {
	fprintf(stdout, "TODO: visit_extend_enum\n");
}

} /* namespace qpssc */
