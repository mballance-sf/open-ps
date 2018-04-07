/*
 * IAction.h
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
 *  Created on: Apr 25, 2016
 *      Author: ballance
 */
#ifndef SRC_API_IACTION_H_
#define SRC_API_IACTION_H_
#include <string>
#include <vector>

#include "IBaseItem.h"
#include "IScopeItem.h"
#include "IActivityStmt.h"
#include "INamedItem.h"

namespace psi_api {
	/**
	 * Specifies the API to an action declaration
	 */
	class IAction :
			public virtual IBaseItem,
			public virtual IScopeItem,
			public virtual INamedItem {

		public:

			virtual ~IAction() { };

			virtual IBaseItem *getSuperType() const = 0;

			/**
			 * Returns the activity body, or null if there is none
			 */
			virtual IActivityStmt *getGraph() const = 0;

			/**
			 * Sets the activity body
			 */
			virtual void setGraph(IActivityStmt *activity) = 0;

	};
}




#endif /* SRC_API_IACTION_H_ */
