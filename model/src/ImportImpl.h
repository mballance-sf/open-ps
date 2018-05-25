/*
 * ImportImpl.h
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
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IMPORTIMPL_H_
#define MODEL_SRC_IMPORTIMPL_H_

#include "IImport.h"
#include "BaseItemImpl.h"



class ImportImpl: public BaseItemImpl, public virtual IImport {
public:

	ImportImpl(IBaseItem *target, bool is_wildcard);

	virtual ~ImportImpl();

	virtual IBaseItem *getTargetType() const { return m_target; }

	virtual bool isWildcard() const { return m_is_wildcard; }

	virtual IBaseItem *clone() const;

private:
	IBaseItem					*m_target;
	bool						m_is_wildcard;
};

#endif /* MODEL_SRC_IMPORTIMPL_H_ */
