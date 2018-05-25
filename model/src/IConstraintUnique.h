/*
 * IConstraintUnique.h
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

#ifndef MODEL_SRC_ICONSTRAINTUNIQUE_H_
#define MODEL_SRC_ICONSTRAINTUNIQUE_H_
#include <vector>
#include "IConstraint.h"
#include "IOpenRangeList.h"
#include "IExpr.h"



class IConstraintUnique : public virtual IConstraint {
public:

	virtual ~IConstraintUnique() { }

	virtual IOpenRangeList *getTarget() const = 0;

};




#endif /* MODEL_SRC_ICONSTRAINTUNIQUE_H_ */
