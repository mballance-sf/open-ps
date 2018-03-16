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
#include <stdarg.h>

ResolveRefsProcessor::ResolveRefsProcessor() {
	m_debug = true;
}

ResolveRefsProcessor::~ResolveRefsProcessor() {
	// TODO Auto-generated destructor stub
}

bool ResolveRefsProcessor::process(IModel *model) {
	bool ret = true;

	try {
		visit_model(model);
	} catch (UndefinedTypeException &e) {
		fprintf(stdout, "Error: Failed to find type %s\n",
				e.type_ref()->toString().c_str());
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

	ret = find_type(type.at(0));

	if (ret && type.size() > 1) {
		IBaseItem *t = ret;
		for (uint32_t i=1; i<type.size(); i++) {
			t = find_type(dynamic_cast<IScopeItem *>(t), type.at(i));

			if (!t) {
				break;
			}
		}
		ret = t;
	}

	return ret;
}

IBaseItem *ResolveRefsProcessor::find_type(const std::string &name) {
	IBaseItem *ret = 0;

	// Search for unqualified type
	for (int32_t i=scopes().size()-1; i>=0; i--) {
		IScopeItem *s = scopes().at(i);
		ret = find_type(s, name);

		if (ret) {
			break;
		}
	}
	return ret;
}

IBaseItem *ResolveRefsProcessor::find_type(
		IScopeItem 			*scope,
		const std::string	&name) {
	IBaseItem *ret = 0;
	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("Search for %s in %p (%s)",name.c_str(),
				scope, (ni)?ni->getName().c_str():"unnamed");
	}

	for (uint32_t i=0; i<scope->getItems().size(); i++) {
		IBaseItem *it = scope->getItems().at(i);
		INamedItem *ni = dynamic_cast<INamedItem *>(it);

		if (m_debug) {
			if (dynamic_cast<IScopeItem *>(it) && ni) {
				debug("  name=%s", ni->getName().c_str());
			}
		}
		if (dynamic_cast<IScopeItem *>(it) &&
				ni && ni->getName() == name) {
			ret = scope->getItems().at(i);
			break;
		}
	}

	return ret;
}

void ResolveRefsProcessor::debug(const char *fmt, ...) {
	va_list ap;
	if (m_debug) {
		va_start(ap, fmt);
		fprintf(stdout, "[DEBUG] ");
		vfprintf(stdout, fmt, ap);
		fprintf(stdout, "\n");
		va_end(ap);
	}
}

void ResolveRefsProcessor::todo(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(stdout, "TODO: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");
	va_end(ap);
}

