/*
 * ExtendEnumImpl.h
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

#ifndef MODEL_SRC_EXTENDENUMIMPL_H_
#define MODEL_SRC_EXTENDENUMIMPL_H_

#include "BaseItemImpl.h"
#include "IExtendEnum.h"



class ExtendEnumImpl:
		public virtual BaseItemImpl,
		public virtual IExtendEnum {
public:
	ExtendEnumImpl(
			IBaseItem							*target,
			const std::vector<IEnumerator *> 	&enumerators);

	virtual ~ExtendEnumImpl();

	virtual ExtendType getExtendType() const {
		return IExtend::ExtendType_Enum;
	}

	virtual IBaseItem *getTarget() const { return m_target; }

	virtual const std::vector<IEnumerator *> &getEnumerators() const {
		return m_enumerators;
	}

	virtual IBaseItem *clone() const;

private:
	IBaseItem							*m_target;
	std::vector<IEnumerator *>			m_enumerators;
};

#endif /* MODEL_SRC_EXTENDENUMIMPL_H_ */
