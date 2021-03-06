/*
 * IEndLocation.h
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
 *  Created on: Mar 22, 2018
 *      Author: ballance
 */

#ifndef MODEL_SRC_ISTARTENDLOCATION_H_
#define MODEL_SRC_ISTARTENDLOCATION_H_
#include "ILocation.h"
#include "IStartLocation.h"

class IStartEndLocation : public virtual IStartLocation {
public:
	virtual ~IStartEndLocation() { }

	virtual const ILocation *getEnd() const = 0;

	virtual void setEnd(ILocation *end) = 0;
};




#endif /* MODEL_SRC_ISTARTENDLOCATION_H_ */
