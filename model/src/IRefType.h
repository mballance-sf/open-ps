/*
 * IRefType.h
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

#ifndef MODEL_SRC_IREFTYPE_H_
#define MODEL_SRC_IREFTYPE_H_
#include <vector>
#include <string>
#include "IBaseItem.h"
#include "IScopeItem.h"



class IRefType : public virtual IBaseItem {
public:

	virtual ~IRefType() { }

	virtual IScopeItem *getScope() const = 0;

	virtual const std::vector<std::string> &getTypeId() const = 0;

	virtual bool fullyQualified() const = 0;

	virtual IBaseItem *getTargetType() const = 0;

	virtual void setTargetType(IBaseItem *) = 0;

	virtual std::string toString() const = 0;

};




#endif /* MODEL_SRC_IREFTYPE_H_ */
