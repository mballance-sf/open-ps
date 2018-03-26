/*
 * ResolveRefsProcessor.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: ballance
 */

#include "ResolveRefsProcessor.h"
#include "UndefinedTypeException.h"
#include "UnresolvedVariableException.h"
#include "IScopeItem.h"
#include <stdio.h>
#include <stdarg.h>

ResolveRefsProcessor::ResolveRefsProcessor() {
	m_debug = true;
	m_phase = 0;
}

ResolveRefsProcessor::~ResolveRefsProcessor() {
	// TODO Auto-generated destructor stub
}

bool ResolveRefsProcessor::process(IModel *model) {

	m_phase = 1; // type-resolution phase
	try {
		visit_model(model);
	} catch (UndefinedTypeException &e) {
		fprintf(stdout, "Error: Failed to find type %s\n",
				e.type_ref()->toString().c_str());
		return false;
	}

	m_phase = 2; // variable-resolution phase
	try {
		visit_model(model);
	} catch (UnresolvedVariableException &e) {
		fprintf(stdout, "Error: Failed to find variable %s in path %s\n",
				e.unresolved_ref()->getId().c_str(),
				e.full_ref()->toString().c_str());
		return false;
	}

	return true;
}

void ResolveRefsProcessor::visit_ref_type(IRefType *ref) {
	if (m_phase == 1) { // type-resolution
		IBaseItem *type = find_type(ref->getTypeId());

		if (!type) {
			throw UndefinedTypeException(scope(), ref);
		}

		ref->setTargetType(type);
	}
}

void ResolveRefsProcessor::visit_variable_ref(IVariableRef *ref) {
	// Only active during phase 2
	if (m_phase == 2) {
		resolve_variable_ref(0, ref, ref);
	}
}

void ResolveRefsProcessor::resolve_variable_ref(
		IScopeItem					*scope,
		IVariableRef				*full_ref,
		IVariableRef				*ref
		) {
	IBaseItem *resolved_ref = 0;

	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("--> resolve_variable_ref %s in scope %s",
				ref->getId().c_str(), (ni)?ni->getName().c_str():"unnamed");
	}


	// If this is the first path element, then
	// we can search up the containing scopes
	if (!scope) {
		for (int32_t i=scopes().size()-1; i>=0; i--) {
			resolved_ref = resolve_variable_ref(
					scopes().at(i), ref->getId());

			if (resolved_ref) {
				break;
			}
		}
	} else {
		resolved_ref = resolve_variable_ref(scope, ref->getId());
	}

	if (!resolved_ref) {
		// TODO: search imports for extensions
		fprintf(stdout, "TODO: search imports for extensions\n");
		for (int32_t i=scopes().size()-1; i>=0; i--) {
			INamedItem *sni = dynamic_cast<INamedItem *>(scopes().at(i));
			fprintf(stdout, "  Scope(1): %s\n",
					(sni)?sni->getName().c_str():"unnamed");
			resolved_ref = resolve_variable_ref_in_ext(
					scopes().at(i), ref->getId());
			if (resolved_ref) {
				break;
			}
		}
//		IScopeItem *s = scope;
//		while (s) {
//			INamedItem *ni = dynamic_cast<INamedItem *>(s);
//			fprintf(stdout, "  Scope: %p %s\n",
//					s, (ni)?ni->getName().c_str():"unnamed");
//
//			s = dynamic_cast<IScopeItem *>(s->getParent());
//		}
	}

	// resolved_ref is a field
	ref->setTarget(resolved_ref);
	if (!resolved_ref) {
		fprintf(stdout, "throw UnresolvedVariable\n");
		throw UnresolvedVariableException(scope, full_ref, ref);
	}

	if (ref->getNext()) {
		// Transform resolved_ref to its type
		IField *ref_field = dynamic_cast<IField *>(resolved_ref);
		IScopeItem *subscope = 0;
		if (dynamic_cast<IRefType *>(ref_field->getDataType())) {
			fprintf(stdout, "is reftype %d\n",
					dynamic_cast<IRefType *>(ref_field->getDataType())->getTargetType()->getType());
			fflush(stdout);
			subscope = dynamic_cast<IScopeItem *>(
					dynamic_cast<IRefType *>(ref_field->getDataType())->getTargetType());
		} else if (dynamic_cast<IArrayType *>(ref_field->getDataType())) {
			fprintf(stdout, "is arraytype\n");
			fflush(stdout);
			subscope = dynamic_cast<IArrayType *>(
					dynamic_cast<IArrayType *>(ref_field->getDataType()));
		} else {
			fprintf(stdout, "Unknown type %d\n",
					ref_field->getDataType()->getType());
			fflush(stdout);
		}
		if (subscope) {
			resolve_variable_ref(subscope, full_ref, ref->getNext());
		} else {
			fprintf(stdout, "Current ref isn't a scope\n");
			throw UnresolvedVariableException(
					scope, full_ref, ref->getNext());
		}
	}

	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("<-- resolve_variable_ref %s in scope %s => %p",
				ref->getId().c_str(), (ni)?ni->getName().c_str():"unnamed",
				resolved_ref);
	}
}

IBaseItem *ResolveRefsProcessor::resolve_variable_ref(
		IScopeItem			*scope,
		const std::string	&id) {
	IBaseItem *ret = 0;

	INamedItem *sn = dynamic_cast<INamedItem *>(scope);

	if (m_debug) {
		debug("  --> Searching scope %s for %s\n",
				(sn)?sn->getName().c_str():"unnamed", id.c_str());
	}

	for (uint32_t i=0; i<scope->getItems().size(); i++) {
		IBaseItem *it = scope->getItems().at(i);
		if (m_debug && dynamic_cast<INamedItem *>(it)) {
			debug("  check %s",
					dynamic_cast<INamedItem *>(it)->getName().c_str());
		}
		if (dynamic_cast<INamedItem *>(it) &&
				dynamic_cast<INamedItem *>(it)->getName() == id) {
			ret = it;
			break;
		} else if (dynamic_cast<IEnumType *>(it)) {
			IEnumType *t = dynamic_cast<IEnumType *>(scope->getItems().at(i));
			for (uint32_t j=0; j<t->getEnumerators().size(); j++) {
				IEnumerator *e = t->getEnumerators().at(j);
				if (e->getName() == id) {
					ret = e;
					break;
				}
			}
		} else if (dynamic_cast<IExtend *>(it) &&
				dynamic_cast<IExtend *>(it)->getExtendType() == IExtend::ExtendType_Enum) {
			// Search here too
			IExtend *ext = dynamic_cast<IExtend *>(it);

		}
	}

	if (!ret) {
		if (dynamic_cast<IAction *>(scope) &&
			dynamic_cast<IAction *>(scope)->getSuperType()) {
			IAction *action = dynamic_cast<IAction *>(scope);
			IRefType *super_r = dynamic_cast<IRefType *>(action->getSuperType());
			IAction *super = dynamic_cast<IAction *>(super_r->getTargetType());

			// Try the super scope
			ret = resolve_variable_ref(super, id);
		} else if (dynamic_cast<IStruct *>(scope) &&
				dynamic_cast<IStruct *>(scope)->getSuperType()) {
			IStruct *s = dynamic_cast<IStruct *>(scope);
			IRefType *super_r = dynamic_cast<IRefType *>(s->getSuperType());
			IStruct *super_s = dynamic_cast<IStruct *>(super_r->getTargetType());

			if (!super_s) {
				fprintf(stdout, "super of type %s is not a struct\n",
						dynamic_cast<INamedItem *>(scope)->getName().c_str());
			}
			ret = resolve_variable_ref(super_s, id);
		} else if (dynamic_cast<IExtend *>(scope)) {
			IExtend *ext = dynamic_cast<IExtend *>(scope);
			IBaseItem *target = dynamic_cast<IRefType *>(ext->getTarget())->getTargetType();
			IScopeItem *target_s = dynamic_cast<IScopeItem *>(target);
			if (m_debug) {
				INamedItem *ni = dynamic_cast<INamedItem *>(target_s);
				debug("Searching extend scope %s", (ni)?ni->getName().c_str():"unnamed");
			}
			ret = resolve_variable_ref(target_s, id);
		}
//				else /*if (dynamic_cast<IComponent *>(scope) &&
//						dynamic_cast<IComponent *>(scope)->get) */ {
//					// Nothing else to check
//					scope = 0;
//				}
	}

	if (m_debug) {
		debug("  <-- Searching scope %s for %s => %p\n",
				(sn)?sn->getName().c_str():"unnamed",
				id.c_str(), ret);
	}

	return ret;
}

IBaseItem *ResolveRefsProcessor::resolve_variable_ref_in_ext(
		IScopeItem			*scope,
		const std::string	&id) {
	IBaseItem *ret = 0;
	if (m_debug) {
		INamedItem *ni = dynamic_cast<INamedItem *>(scope);
		debug("--> resolve_variable_ref_in_ext %s\n",
				(ni)?ni->getName().c_str():"unnamed");
	}
	for (std::vector<IBaseItem *>::const_iterator it=scope->getItems().begin();
			it!=scope->getItems().end(); it++) {
		if ((*it)->getType() == IBaseItem::TypeExtend) {
			IExtend *ext = dynamic_cast<IExtend *>(*it);
			if (m_debug) {
				debug("  -- Extension %d", ext->getExtendType());
			}
			if (ext->getExtendType() == IExtend::ExtendType_Enum) {
				IExtendEnum *ext_e = dynamic_cast<IExtendEnum *>(ext);
				for (std::vector<IEnumerator *>::const_iterator it_e=ext_e->getEnumerators().begin();
						it_e!=ext_e->getEnumerators().end(); it_e++) {
					if ((*it_e)->getName() == id) {
						ret = *it_e;
						break;
					}
				}
			} else {
				IExtendComposite *ext_c = dynamic_cast<IExtendComposite *>(ext);
				// First, search the scope
				for (std::vector<IBaseItem *>::const_iterator it_c=ext_c->getItems().begin();
						it_c!=ext_c->getItems().end(); it_c++) {
					INamedItem *ni = dynamic_cast<INamedItem *>(*it_c);
					if (m_debug) {
						debug("  -- Field %s", (ni)?ni->getName().c_str():"unnamed");
					}
					if (ni && ni->getName() == id) {
						ret = *it_c;
						break;
					}
				}
			}
		} else if ((*it)->getType() == IBaseItem::TypeImport) {
			IImport *imp = dynamic_cast<IImport *>(*it);
			if (imp->getTargetType()) {
				IRefType *ref_t = dynamic_cast<IRefType *>(imp->getTargetType());
				if (dynamic_cast<IScopeItem *>(ref_t->getTargetType())) {
					ret = resolve_variable_ref_in_ext(
							dynamic_cast<IScopeItem *>(ref_t->getTargetType()), id);
				} else {
					fprintf(stdout, "Error: import target is not a scope\n");
				}
			} else {
				fprintf(stdout, "Error: import target type not resolved\n");
			}
		}

		if (ret) {
			break;
		}
	}

	if (m_debug) {
		debug("<-- resolve_variable_ref_in_ext %p\n", ret);
	}

	return ret;
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

