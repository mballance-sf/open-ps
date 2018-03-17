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
	m_debug = false;
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

	if (m_debug) {
		debug("--> find_type %s", type2string(type).c_str());
	}

	ret = find_type(type.at(0));

	if (ret && type.size() > 1) {
		IBaseItem *t = ret;
		for (uint32_t i=1; i<type.size(); i++) {
//			if (!dynamic_cast<IScopeItem *>(t)) {
//				fprintf(stdout, "Error: %p (%d) is not a scope item\n",))
//			}
			if (dynamic_cast<IScopeItem *>(t)) {
				t = find_type(dynamic_cast<IScopeItem *>(t), type.at(i));
			} else {
				fprintf(stdout, "t isn't an IScopeItem\n");
				t = 0;
			}

			if (t) {
				debug("  ... found type element %s", type.at(i).c_str());
			} else {
				debug("  ... failed to find type element %s", type.at(i).c_str());
			}
			if (!t) {
				break;
			}
		}
		ret = t;
	}

	if (m_debug) {
		debug("<-- find_type %s => %p", type2string(type).c_str(), ret);
	}

	return ret;
}

IBaseItem *ResolveRefsProcessor::find_type(const std::string &name) {
	IBaseItem *ret = 0;

	if (m_debug) {
		debug("--> Search up scope for %s", name.c_str());
	}

	// Search for unqualified type
	for (int32_t i=scopes().size()-1; i>=0; i--) {
		IScopeItem *s = scopes().at(i);

		if (m_debug) {
			debug("--> Search scope %d\n", i);
		}
		ret = find_type(s, name);

		if (m_debug) {
			debug("<-- Search scope %d => %p\n", i, ret);
		}

		if (ret) {
			break;
		}
	}

	if (m_debug) {
		debug("<-- Search up scope for %s => %p", name.c_str(), ret);
	}

	return ret;
}

IBaseItem *ResolveRefsProcessor::find_type(
		IScopeItem 			*scope,
		const std::string	&name) {
	IBaseItem *ret = 0;
	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("--> Search for %s in %p (%s)",name.c_str(),
				scope, (ni)?ni->getName().c_str():"unnamed");
	}

	if (is_type(scope) &&
			dynamic_cast<INamedItem *>(scope)->getName() == name) {
		// Scope must be something like a package, component, or action name
		ret = dynamic_cast<IBaseItem *>(scope);
	} else if (dynamic_cast<IExtend *>(scope) &&
			dynamic_cast<IExtend *>(scope)->getExtendType() != IExtend::ExtendType_Enum) {
		// Find the target type and search it
		IExtend *ext = dynamic_cast<IExtend *>(scope);
		IBaseItem *ext_target = dynamic_cast<IRefType *>(ext->getTarget())->getTargetType();

		if (dynamic_cast<INamedItem *>(ext_target)->getName() == name) {
			ret = ext_target;
		} else {
			ret = find_type(dynamic_cast<IScopeItem *>(ext_target), name);
		}
	} else {
		for (uint32_t i=0; i<scope->getItems().size(); i++) {
			IBaseItem *it = scope->getItems().at(i);

			if (it->getType() == IBaseItem::TypeImport) {
				IImport *imp = dynamic_cast<IImport *>(it);

				// Search the scope if this is a wildcard
				if (imp->isWildcard()) {
					IRefType *ref = dynamic_cast<IRefType *>(imp->getTargetType());

					if (ref->getTargetType()) {
						ret = find_type(dynamic_cast<IScopeItem *>(ref->getTargetType()), name);
					}
				} else {
					// Test for equality of not
					todo("test import equality");
				}
			} else if (is_type(it)) {
				debug("  name=%s target_name=%s",
						dynamic_cast<INamedItem *>(it)->getName().c_str(),
						name.c_str());
				if (dynamic_cast<INamedItem *>(it)->getName() == name) {
					debug("    ... found");
					ret = it;
					break;
				}
			}
		}
	}

	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("<-- Search for %s in %p (%s) => %p",name.c_str(),
				scope, (ni)?ni->getName().c_str():"unnamed", ret);
	}

	return ret;
}

bool ResolveRefsProcessor::is_type(IBaseItem *it) {
	return (dynamic_cast<INamedItem *>(it) &&
			(dynamic_cast<IScopeItem *>(it) ||
					dynamic_cast<IEnumType *>(it)));
}

bool ResolveRefsProcessor::is_type(IScopeItem *it) {
	return (dynamic_cast<INamedItem *>(it));
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

std::string ResolveRefsProcessor::type2string(const std::vector<std::string> &type) {
	std::string ret;

	for (uint32_t i=0; i<type.size(); i++) {
		ret.append(type.at(i));
		if (i+1 < type.size()) {
			ret.append("::");
		}
	}

	return ret;
}

