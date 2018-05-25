/*
 * UnresolvedVariableException.h
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
