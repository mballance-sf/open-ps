/*
 * ResolveRefsProcessor.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */

#include "ResolveRefsProcessor.h"
#include "UndefinedTypeException.h"
#include "IScopeItem.h"
#include <stdio.h>

ResolveRefsProcessor::ResolveRefsProcessor() {
	// TODO Auto-generated constructor stub

}

ResolveRefsProcessor::~ResolveRefsProcessor() {
	// TODO Auto-generated destructor stub
}

bool ResolveRefsProcessor::process(IModel *model) {
	bool ret = true;

	try {
		visit_model(model);
	} catch (UndefinedTypeException e) {
		fprintf(stdout, "Error: Failed to find type\n");
		ret = false;
	}

	return true;
}

void ResolveRefsProcessor::visit_ref_type(IRefType *ref) {
	IBaseItem *type = find_type(ref->getTypeId());

	if (!type) {
		throw UndefinedTypeException(scope(), ref);
	}

	ref->setTargetType(type);
}

IBaseItem *ResolveRefsProcessor::find_type(
		const std::vector<std::string> &type) {
	IBaseItem *ret = 0;

	if (type.size() > 1) {
	} else {
		// Search for unqualified type
		for (int32_t i=scopes().size()-1; i>=0; i--) {
			IScopeItem *s = scopes().at(i);
			ret = find_type(s, type.at(0));

			if (ret) {
				break;
			}
		}
	}

	return ret;
}

IBaseItem *ResolveRefsProcessor::find_type(
		IScopeItem 			*scope,
		const std::string	&name) {
	IBaseItem *ret = 0;
//	fprintf(stdout, "Search for %s in %p\n",name.c_str(), scope);

	for (uint32_t i=0; i<scope->getItems().size(); i++) {
		IBaseItem *it = scope->getItems().at(i);
		INamedItem *ni = dynamic_cast<INamedItem *>(it);

//		if (dynamic_cast<IScopeItem *>(it) && ni) {
//			fprintf(stdout, "  name=%s\n", ni->getName().c_str());
//		}
		if (dynamic_cast<IScopeItem *>(it) &&
				ni && ni->getName() == name) {
			ret = scope->getItems().at(i);
			break;
		}
	}

	return ret;
}
