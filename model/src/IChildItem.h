/*
 * IChildItem.h
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

#pragma once

class IBaseItem;
class IChildItem {
public:
	virtual ~IChildItem() { }

	virtual IBaseItem *getParent() const = 0;

	/**
	 * Implementation: not called by user code
	 */
	virtual void setParent(IBaseItem *it) = 0;

};



