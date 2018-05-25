/*
 * IArrayType.h
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
 *  Created on: Mar 21, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_IARRAYTYPE_H_
#define MODEL_SRC_IARRAYTYPE_H_
#include "IBaseItem.h"
#include "IScopeItem.h"
#include "IField.h"
#include "IExpr.h"



class IArrayType : public virtual IBaseItem, public virtual IScopeItem {
public:

	virtual ~IArrayType() { }

	virtual bool hasSum() const = 0;

	virtual IField *getSize() const = 0;

	virtual IField *getSum() const = 0;

	virtual IBaseItem *getTargetType() const = 0;

	virtual IExpr *getLhs() const = 0;

	virtual IExpr *getRhs() const = 0;

};




#endif /* MODEL_SRC_IARRAYTYPE_H_ */
