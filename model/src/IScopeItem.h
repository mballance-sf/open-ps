/*
 * IScopeItem.h
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
 *
 *  Created on: May 2, 2016
 *      Author: ballance
 */

#pragma once

#include <string>
#include <vector>

#include "IBaseItem.h"
#include "IStartEndLocation.h"
#include "IChildItem.h"

	class IField;

	class IScopeItem :
			public virtual IChildItem,
			public virtual IStartEndLocation {
	public:

		virtual ~IScopeItem() { }

		virtual const std::vector<IBaseItem *> &getItems() const = 0;

		virtual void add(IBaseItem *item) = 0;

		/**
		 * Locates and returns the named field. Returns 0 if
		 * the named field does not exist
		 */
		virtual IField *getField(const std::string &name) = 0;

	};
