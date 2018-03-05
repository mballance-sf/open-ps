/*
 * MarkReferencedTypesProcessor.cpp
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
