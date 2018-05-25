/*
 * SymbolImpl.h
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
 *  Created on: Mar 26, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_SYMBOLIMPL_H_
#define MODEL_SRC_SYMBOLIMPL_H_

#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "ISymbol.h"




class SymbolImpl: public ISymbol,
	public virtual BaseItemImpl,
	public virtual ScopeItemImpl {
public:
	SymbolImpl(
			const std::string				&name,
			const std::vector<IField *>		&params,
			IActivityBlockStmt					*body
			);

	virtual ~SymbolImpl();

	virtual const std::string &getName() const { return m_name; }

	virtual const std::vector<IField *> &getParameters() const { return m_params; }

	virtual IActivityBlockStmt *getBody() const { return m_body; }

	virtual IBaseItem *clone() const;

private:
	std::string						m_name;
	std::vector<IField *>			m_params;
	IActivityBlockStmt					*m_body;

};

#endif /* MODEL_SRC_SYMBOLIMPL_H_ */
