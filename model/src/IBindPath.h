/*
 * IBindPath.h
 *
 * Copyright 2016 Mentor Graphics Corporation
 * All Rights Reserved Worldwide
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
 *  Created on: Jul 22, 2016
 *      Author: ballance
 */

#pragma once
#include <vector>

class IBaseItem;
class IBindPath {
public:
	virtual ~IBindPath() { }

	virtual const std::vector<IBaseItem *> &getPath() const = 0;

	virtual IBindPath *clone() const = 0;

};

