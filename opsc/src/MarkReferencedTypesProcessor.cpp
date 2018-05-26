/*
 * MarkReferencedTypesProcessor.cpp
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

#include "MarkReferencedTypesProcessor.h"

namespace qpssc {

MarkReferencedTypesProcessor::MarkReferencedTypesProcessor() {
	// TODO Auto-generated constructor stub

}

MarkReferencedTypesProcessor::~MarkReferencedTypesProcessor() {
	// TODO Auto-generated destructor stub
}

bool MarkReferencedTypesProcessor::process(IModel *model) {
	visit_model(model);
	return true;
}

void MarkReferencedTypesProcessor::visit_action(IAction *a) {
	if (a->getSuperType()) {
		a->getSuperType()->setAttribute("REFERENCED", "true");
	}
}

void MarkReferencedTypesProcessor::visit_struct(IStruct *str) {
	if (str->getSuperType()) {
		str->getSuperType()->setAttribute("REFERENCED", "true");
	}
}

void MarkReferencedTypesProcessor::visit_component(IComponent *c) {
	// TODO: need notion of super-type for component
//	if (c->getSuperType()) {
//		c->getSuperType()->setAttribute("REFERENCED", "true");
//	}
}

void MarkReferencedTypesProcessor::visit_field(IField *f) {
	IBaseItem *t = f->getDataType();

	if (t->getType() == IBaseItem::TypeAction ||
			t->getType() == IBaseItem::TypeComponent ||
			t->getType() == IBaseItem::TypeStruct) {
		t->setAttribute("REFERENCED", "true");

		// Also mark the containing scopes as referenced
		// In other words, the containing package is required if the action is required
		while (t) {
			t->setAttribute("REFERENCED", "true");
			t = t->getParent();
		}
	}
}

} /* namespace qpssc */
