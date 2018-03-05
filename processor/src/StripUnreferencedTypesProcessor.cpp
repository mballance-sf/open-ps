/*
 * StripUnreferencedTypesProcessor.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: ballance
 */

#include "StripUnreferencedTypesProcessor.h"

namespace qpssc {

StripUnreferencedTypesProcessor::StripUnreferencedTypesProcessor() {
	// TODO Auto-generated constructor stub

}

StripUnreferencedTypesProcessor::~StripUnreferencedTypesProcessor() {
	// TODO Auto-generated destructor stub
}

bool StripUnreferencedTypesProcessor::process(IModel *model) {
	visit_model(model);
	return true;
}

void StripUnreferencedTypesProcessor::visit_action(IAction *a) {
	if (!a->hasAttribute("REFERENCED")) {
		remove();
	}
}

void StripUnreferencedTypesProcessor::visit_struct(IStruct *str) {
	if (!str->hasAttribute("REFERENCED")) {
		remove();
	}
}

void StripUnreferencedTypesProcessor::visit_component(IComponent *c) {
	if (!c->hasAttribute("REFERENCED")) {
		remove();
	}
}

void StripUnreferencedTypesProcessor::visit_package(IPackage *pkg) {
	PSIVisitor::visit_package(pkg);

	if (pkg->getItems().size() == 0) {
		// Empty package
		remove();
	}

}


} /* namespace qpssc */
