/*
 * IVariableRef.h
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

#ifndef MODEL_SRC_IVARIABLEREF_H_
#define MODEL_SRC_IVARIABLEREF_H_
#include "IExpr.h"
#include "IScopeItem.h"
#include <string>



class IVariableRef : public virtual IExpr {
public:

	virtual ~IVariableRef() { }

	virtual IBaseItem *getScope() const = 0;

	virtual const std::string &getId() const = 0;

	virtual IExpr *getIndexLhs() const = 0;

	virtual IExpr *getIndexRhs() const = 0;

	virtual IBaseItem *getTarget() const = 0;

	virtual void setTarget(IBaseItem *t) = 0;

	virtual void setNext(IVariableRef *next) = 0;

	// Returns the next link in the variable-ref chain
	virtual IVariableRef *getNext() const = 0;

	virtual std::string toString() const = 0;

};




#endif /* MODEL_SRC_IVARIABLEREF_H_ */
