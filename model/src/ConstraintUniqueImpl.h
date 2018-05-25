/*
 * ConstraintUniqueImpl.h
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
 *  Created on: Mar 31, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_
#define MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_

#include "IConstraintUnique.h"
#include "BaseItemImpl.h"



class ConstraintUniqueImpl: public virtual IConstraintUnique, public virtual BaseItemImpl {
public:

	ConstraintUniqueImpl(IOpenRangeList	*target);

	virtual ~ConstraintUniqueImpl();

	virtual ConstraintType getConstraintType() const {
		return IConstraint::ConstraintType_Unique;
	}

	virtual IOpenRangeList *getTarget() const { return m_target; }

	virtual IBaseItem *clone() const;

private:
	IOpenRangeList					*m_target;
};

#endif /* MODEL_SRC_CONSTRAINTUNIQUEIMPL_H_ */
