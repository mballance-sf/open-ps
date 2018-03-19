/*
 * UnresolvedVariableException.h
 *
 *  Created on: Mar 18, 2018
 *      Author: ballance
 */

#ifndef PROCESSOR_SRC_UNRESOLVEDVARIABLEEXCEPTION_H_
#define PROCESSOR_SRC_UNRESOLVEDVARIABLEEXCEPTION_H_
#include <exception>
#include "IVariableRef.h"
#include "IScopeItem.h"

class UnresolvedVariableException : public std::exception {
public:

	UnresolvedVariableException(
			IScopeItem			*scope,
			IVariableRef		*full_ref,
			IVariableRef		*unresolved_ref);

	virtual ~UnresolvedVariableException();

	IScopeItem *scope() const { return m_scope; }

	IVariableRef *full_ref() const { return m_full_ref; }

	IVariableRef *unresolved_ref() const { return m_unresolved_ref; }

private:

	IScopeItem					*m_scope;
	IVariableRef				*m_full_ref;
	IVariableRef				*m_unresolved_ref;

};

#endif /* PROCESSOR_SRC_UNRESOLVEDVARIABLEEXCEPTION_H_ */
