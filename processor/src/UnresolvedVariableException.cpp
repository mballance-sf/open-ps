/*
 * UnresolvedVariableException.cpp
 *
 *  Created on: Mar 18, 2018
 *      Author: ballance
 */

#include "UnresolvedVariableException.h"

UnresolvedVariableException::UnresolvedVariableException(
		IScopeItem				*scope,
		IVariableRef			*full_ref,
		IVariableRef			*unresolved_ref) :
	m_scope(scope), m_full_ref(full_ref),
	m_unresolved_ref(unresolved_ref) {
	// TODO Auto-generated constructor stub

}

UnresolvedVariableException::~UnresolvedVariableException() {
	// TODO Auto-generated destructor stub
}

