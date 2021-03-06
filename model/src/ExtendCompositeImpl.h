/*
 * ExtendCompositeImpl.h
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
 *  Created on: Mar 24, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_EXTENDCOMPOSITEIMPL_H_
#define MODEL_SRC_EXTENDCOMPOSITEIMPL_H_

#include "BaseItemImpl.h"
#include "ScopeItemImpl.h"
#include "IExtendComposite.h"



class ExtendCompositeImpl:
		public virtual IExtendComposite,
		public virtual BaseItemImpl,
		public virtual ScopeItemImpl {
public:
	ExtendCompositeImpl(
			IExtend::ExtendType	type,
			IBaseItem			*target);

	virtual ~ExtendCompositeImpl();

	virtual ExtendType getExtendType() const { return m_extend_type; }

	virtual IBaseItem *getTarget() const { return m_target; }

	virtual IBaseItem *clone() const;

private:
	IExtend::ExtendType				m_extend_type;
	IBaseItem						*m_target;
};

#endif /* MODEL_SRC_EXTENDCOMPOSITEIMPL_H_ */
