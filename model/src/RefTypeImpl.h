/*
 * RefTypeImpl.h
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
 *  Created on: Mar 9, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_REFTYPEIMPL_H_
#define MODEL_SRC_REFTYPEIMPL_H_

#include "IRefType.h"
#include "BaseItemImpl.h"


class RefTypeImpl: public BaseItemImpl,
	public virtual IRefType {
public:
	RefTypeImpl(
			IScopeItem 						*scope,
			const std::vector<std::string>	&path,
			bool							fully_qualified);

	virtual ~RefTypeImpl();

	virtual IScopeItem *getScope() const;

	virtual const std::vector<std::string> &getTypeId() const;

	virtual bool fullyQualified() const { return m_fully_qualified; }

	virtual IBaseItem *getTargetType() const;

	virtual void setTargetType(IBaseItem *t);

	virtual IBaseItem *clone() const;

	virtual std::string toString() const;

private:

	IScopeItem					*m_scope;
	std::vector<std::string>	m_typeid;
	bool						m_fully_qualified;
	IBaseItem					*m_target;

};

#endif /* MODEL_SRC_REFTYPEIMPL_H_ */
